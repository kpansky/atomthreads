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
#include "xparameters.h"
#include "mb_interface.h"
#include "xtmrctr.h"
#include "xintc.h"

static XIntc myIntc;
static XTmrCtr myTmrCtr;
volatile uint32_t myTmrCtrIntOccurred;

/**
 * \b microblazeInterruptWrapper
 *
 * Installs a wrapper into the interrupt handler
 * 
 * Wraps the normal interrupt handler with AtomThread conformant
 * critical sections.  The timer tick is not called from inside the
 * interrupt handler directly to prevent scheduling a thread before
 * all interrupts pending are handled.
 *
 * @param[in] DataPtr - device ID of the XIntc device
 *
 * @return None
 */
void microblazeInterruptWrapper(void *DataPtr)
{
	atomIntEnter();
	myTmrCtrIntOccurred = FALSE;
	XIntc_DeviceInterruptHandler(DataPtr);
	if (myTmrCtrIntOccurred == TRUE)
	{
		atomTimerTick();
	}
	atomIntExit(TRUE);
}

/**
 * \b microblazeTimerHandler
 *
 * Interrupt handler for the timer tick.
 *
 * @param[in] CallbackRef - unused
 * @param[in] TmrCtrNumber timer number that caused this interrupt
 *
 * @return None
 */
void microblazeTimerHandler(void *CallBackRef, uint8_t TmrCtrNumber)
{ 
  if (TmrCtrNumber == 0) 
  {
  	myTmrCtrIntOccurred = TRUE;
  }
}

/**
 * \b microblazeInitSystemTickTimer
 *
 * Initialise the system tick timer.
 *
 * @return None
 */
void microblazeInitSystemTickTimer ( void )
{
  /* Install our wrapper to handle AtomThread prologue/epilogue of interrupts */
  microblaze_register_handler(microblazeInterruptWrapper, XPAR_XPS_INTC_0_DEVICE_ID);

  /* Setup the interrupt controller with the timer enabled */  
  XIntc_Initialize(&myIntc, XPAR_XPS_INTC_0_DEVICE_ID);
  XIntc_Connect(&myIntc, XPAR_XPS_INTC_0_XPS_TIMER_0_INTERRUPT_INTR,
                (XInterruptHandler)XTmrCtr_InterruptHandler, &myTmrCtr);
  XIntc_Start(&myIntc, XIN_REAL_MODE);
  XIntc_Enable(&myIntc, XPAR_XPS_INTC_0_XPS_TIMER_0_INTERRUPT_INTR);

  /* Set our timer to generate automatic interrupts at a SYSTEM_TICKS_PER_SEC rate */
  XTmrCtr_Initialize(&myTmrCtr, XPAR_XPS_TIMER_0_DEVICE_ID);
  XTmrCtr_SetOptions(&myTmrCtr, 0, XTC_DOWN_COUNT_OPTION | XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);
  XTmrCtr_SetHandler(&myTmrCtr, (XTmrCtr_Handler)microblazeTimerHandler, NULL);
  XTmrCtr_SetResetValue(&myTmrCtr, 0, XPAR_CPU_CORE_CLOCK_FREQ_HZ / SYSTEM_TICKS_PER_SEC);
  XTmrCtr_Start(&myTmrCtr, 0);
}
