/*
 * Copyright (c) 2012, Kevin Pansky for Atomthreads Project. 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. No personal names or organizations' names associated with the
 *    Atomthreads project may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE ATOMTHREADS PROJECT AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "atom.h"
#include "atomtests.h"
#include "atomtimer.h"
#include <stdarg.h>

/* Constants */

/*
 * Idle thread stack size
 *
 * This needs to be large enough to handle any interrupt handlers
 * and callbacks called by interrupt handlers (e.g. user-created
 * timer callbacks) as well as the saving of all context when
 * switching away from this thread.
 *
 * In this case, the idle stack is allocated on the BSS via the
 * idle_thread_stack[] byte array.
 */
#define IDLE_STACK_SIZE_BYTES       512


/*
 * Main thread stack size
 *
 * Note that this is not a required OS kernel thread - you will replace
 * this with your own application thread.
 *
 * In this case the Main thread is responsible for calling out to the
 * test routines. Once a test routine has finished, the test status is
 * printed out on the UART and the thread remains running in a wait loop.
 *
 * The Main thread stack generally needs to be larger than the idle
 * thread stack, as not only does it need to store interrupt handler
 * stack saves and context switch saves, but the application main thread
 * will generally be carrying out more nested function calls and require
 * stack for application code local variables etc.
 * 
 */
#define MAIN_STACK_SIZE_BYTES       1024

/* Local data */

/* Application threads' TCBs */
static ATOM_TCB main_tcb;

/* Main thread's stack area */
static uint32_t main_thread_stack[MAIN_STACK_SIZE_BYTES/4];

/* Idle thread's stack area */
static uint32_t idle_thread_stack[IDLE_STACK_SIZE_BYTES/4];

/* Forward declarations */
static void main_thread_func (uint32_t data);
void microblazeInitSystemTickTimer(void);
void log_itoa(int);

/**
 * \b main
 *
 * Program entry point.
 *
 * Sets up the MicroBlaze hardware resources (system tick timer interrupt) necessary
 * for the OS to be started. Creates an application thread and starts the OS.
 */
int main ( void )
{
    int8_t status;

    ATOMLOG("\n\nAtomThreads MicroBlaze Starting\n\n");

    /**
     * Initialise the OS before creating our threads.
     */
    status = atomOSInit(&idle_thread_stack[0], IDLE_STACK_SIZE_BYTES, TRUE);
    if (status == ATOM_OK)
    {
        /* Enable the system tick timer */
        microblazeInitSystemTickTimer();

        /* Create an application thread */
        status = atomThreadCreate(&main_tcb,
                     TEST_THREAD_PRIO, main_thread_func, 0,
                     &main_thread_stack[0],
                     MAIN_STACK_SIZE_BYTES,
                     TRUE);

        if (status == ATOM_OK)
        {
            /**
             * First application thread successfully created. It is
             * now possible to start the OS. Execution will not return
             * from atomOSStart(), which will restore the context of
             * our application thread and start executing it.
             *
             * Note that interrupts are still disabled at this point.
             * They will be enabled as we restore and execute our first
             * thread in archFirstThreadRestore().
             */
            atomOSStart();
        }
    }

    while (1)
        ;

    /* There was an error starting the OS if we reach here */
    return (0);
}


/**
 * \b main_thread_func
 *
 * Entry point for main application thread.
 *
 * This is the first thread that will be executed when the OS is started.
 *
 * @param[in] data Unused (optional thread entry parameter)
 *
 * @return None
 */
static void main_thread_func (uint32_t data)
{
    uint32_t test_status;

    /* Put a message out on the UART */
    ATOMLOG("Go\n");

    /* Start test. All tests use the same start API. */
    test_status = test_start();

    /* Check main thread stack usage (if enabled) */
#ifdef ATOM_STACK_CHECKING
    if (test_status == 0)
    {
        uint32_t used_bytes, free_bytes;

        /* Check idle thread stack usage */
        if (atomThreadStackCheck (&main_tcb, &used_bytes, &free_bytes) == ATOM_OK)
        {
            /* Check the thread did not use up to the end of stack */
            if (free_bytes == 0)
            {
                ATOMLOG("Main stack overflow\n");
                test_status++;
            }

            /* Log the stack usage */
#ifdef TESTS_LOG_STACK_USAGE
            ATOMLOG("MainUse:%d\n", used_bytes);
#endif
        }

    }
#endif

    /* Log final status */
    if (test_status == 0)
    {
        ATOMLOG("Pass\n");
    }
    else
    {
        ATOMLOG("Fail(%d)\n", test_status);
    }

    /* Test finished, sit idle */
    while (1)
    {
        atomTimerDelay(SYSTEM_TICKS_PER_SEC);
    }
}

/**
 * \b ATOMLOG
 *
 * Printf like function loggint to stdout.
 *
 * Behaves similar to printf, but can accept only the simplest formats.
 * Has the advantage of requiring relatively little stack space.
 *
 * @param[in] format The printf-style format string
 * @param[in] ... The variable argument list
 *
 * @return None
 */
void ATOMLOG(char *format, ...)
{
  CRITICAL_STORE;
  int ii=0;
  int val_d = 0;
  char *val_s = 0;
  va_list ap;

  CRITICAL_START();

  va_start(ap, format);

  for (ii=0; format[ii] != 0; ii++)
  {
    if (format[ii] == '%')
    {
      if (format[++ii] == 'd')
      {
        val_d = va_arg(ap,int);
        log_itoa(val_d);
      }
      else if (format[++ii] == 's')
      {
        val_s = va_arg(ap,char*);
        puts(val_s);
      } 
      else 
      {
        puts("%ERR");
      }
    }
    else
    {
      if (format[ii] == '\n') putchar('\r');
      putchar(format[ii]);
    }
  }

  va_end(ap);

  CRITICAL_END();
}


/**
 * \b log_itoa
 *
 * Print an integer to stdout.
 *
 * Used for the ATOMLOG function.
 *
 * @param[in] data The signed integer value to convert
 *
 * @return None
 */
void log_itoa(int integer)
{
  /* Largest base 10 divisor that fits in 32 bits */
  int div = 1000000000;
  
  /* Did we encounter MSB yet */
  int fde = 0;

  int rem = 0;

  /* If we're zero, report so */
  if (integer == 0) {
    putchar('0');
    return;
  }

  /* If we're negative, report so and make it positive */
  if (integer < 0) {
    integer = ~integer;
    integer += 1;
    putchar('-');
  }

  /* Divide and report until we have no more digits */
  while (integer != 0) {

    rem = integer / div;

    if (rem != 0) fde = 1;

    if (fde == 1) {
      putchar('0' + rem);
      integer -= rem * div;
    }
    div /= 10;
  }
  
  return;
}


