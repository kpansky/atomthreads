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

#define AC_MSR 0
#define AC_R31 4
#define AC_R30 8
#define AC_R29 12
#define AC_R28 16
#define AC_R27 20
#define AC_R26 24
#define AC_R25 28
#define AC_R24 32
#define AC_R23 36
#define AC_R22 40
#define AC_R21 44
#define AC_R20 48
#define AC_R19 52
#define AC_R18 56
#define AC_R17 60
#define AC_R16 64
#define AC_R15 68
#define AC_R14 72
#define AC_R13 76
#define AC_R12 80
#define AC_R11 84
#define AC_R10 88
#define AC_R9 92
#define AC_R8 96
#define AC_R7 100
#define AC_R6 108
#define AC_R5 112
#define AC_R4 116
#define AC_R3 120
#define AC_R2 124
#define AC_SIZE 128

/* keep only the ICE,DCE, fields.  MMU not supported currently */
#define MSR_DEFAULT_MASK 0x00000042 
#define MSR_IE 0x00000002

  .section .text
  
  
.macro STORE_CONTEXT

  /* Make room for the context on the stack. */
  addik r1, r1, -AC_SIZE
	
  /* Store registers */
  swi r31, r1, AC_R31
  swi r30, r1, AC_R30
  swi r29, r1, AC_R29
  swi r28, r1, AC_R28
  swi r27, r1, AC_R27
  swi r26, r1, AC_R26
  swi r25, r1, AC_R25
  swi r24, r1, AC_R24
  swi r23, r1, AC_R23
  swi r22, r1, AC_R22
  swi r21, r1, AC_R21
  swi r20, r1, AC_R20
  swi r19, r1, AC_R19
  swi r18, r1, AC_R18
  swi r17, r1, AC_R17
  swi r16, r1, AC_R16
  swi r15, r1, AC_R15
  swi r14, r1, AC_R14
  swi r13, r1, AC_R13
  swi r12, r1, AC_R12
  swi r11, r1, AC_R11
  swi r10, r1, AC_R10
  swi r9, r1, AC_R9
  swi r8, r1, AC_R8
  swi r7, r1, AC_R7
  swi r6, r1, AC_R6
  swi r5, r1, AC_R5
  swi r4, r1, AC_R4
  swi r3, r1, AC_R3
  swi r2, r1, AC_R2

  /* Store the msr */
  mfs r31, rmsr
  swi r31, r1, AC_MSR
  lwi r31, r1, AC_R31
  
.endm

.macro LOAD_CONTEXT

  /* Load general registers. */
  lwi r31, r1, AC_R31
  lwi r30, r1, AC_R30
  lwi r29, r1, AC_R29
  lwi r28, r1, AC_R28
  lwi r27, r1, AC_R27
  lwi r26, r1, AC_R26
  lwi r25, r1, AC_R25
  lwi r24, r1, AC_R24
  lwi r23, r1, AC_R23
  lwi r22, r1, AC_R22
  lwi r21, r1, AC_R21
  lwi r20, r1, AC_R20
  lwi r19, r1, AC_R19
  lwi r18, r1, AC_R18
  lwi r17, r1, AC_R17
  lwi r16, r1, AC_R16
  lwi r15, r1, AC_R15
  lwi r14, r1, AC_R14
  lwi r13, r1, AC_R13
  lwi r12, r1, AC_R12
  lwi r11, r1, AC_R11
  lwi r10, r1, AC_R10
  lwi r9, r1, AC_R9
  lwi r8, r1, AC_R8
  lwi r7, r1, AC_R7
  lwi r6, r1, AC_R6
  lwi r5, r1, AC_R5
  lwi r4, r1, AC_R4
  lwi r3, r1, AC_R3
  lwi r2, r1, AC_R2

  /* Clear interrupt flag -- enabled by RTID */
  lwi r31, r1, AC_MSR
  andni r31, r31, MSR_IE
  mts rmsr, r31
	
  /* Load the MSR for testing */
  lwi r31, r1, AC_MSR
  /* Check for interrupts enabled in the context's MSR */
  /* If interrupts not enabled, jump forward and keep disabled */
  /* This can happen if a cooperative context switch happens 
     in a critical section
     */
  andi r31, r31, MSR_IE
  beqid r31, 1f
  lwi r31, r1, AC_R31

  rtid r15, 0
  addik r1, r1, AC_SIZE

1:
  rtsd r15, 0
  addik r1, r1, AC_SIZE

.endm


/**
 * \b archThreadContextInit
 *
 * Architecture-specific thread context initialisation routine.
 *
 * This function must set up a thread's context ready for restoring
 * and running the thread via archFirstThreadRestore() or
 * archContextSwitch().
 *
 * The layout required to fill the correct register values is
 * described in archContextSwitch().
 *
 * @param[in] tcb_ptr Pointer to the TCB of the thread being created
 * @param[in] stack_top Pointer to the top of the new thread's stack
 * @param[in] entry_point Pointer to the thread entry point function
 * @param[in] entry_param Parameter to be passed to the thread entry point
 *
 * @return None
 *
 * void archThreadContextInit (ATOM_TCB *tcb_ptr, void *stack_top, 
 *                             void (*entry_point)(UINT32), 
 *                             UINT32 entry_param)
 */
.global archThreadContextInit
archThreadContextInit:
  swi r5, r1, 4
  swi r6, r1, 8
  swi r7, r1, 12
  swi r8, r1, 16
  addik	r1, r1, -8
  swi r19, r1, 4
  swi r15, r1, 0

  /* R5 - tcb_ptr */
  /* R6 - stack_top */
  /* R7 - entry_point */
  /* R8 - entry_param */

  /*Copy R1 to R19*/
  addk r19, r1, r0 
  
  /*Copy stack_top to R1 for context store*/
  addik r1, r6, 0
  
  /* Create stack space in the new TCB for the (non-existant)
     "calling" function 
     */
  addik r1, r1, -12
  swi r0, r1, 8 /* R19 */
  swi r8, r1, 4 /* entry_param */
  swi r0, r1, 0 /* link pointer -- should never happen.  System reset will occur... */
  
  /* Save context to stack */
  STORE_CONTEXT
  
  /* Store the final stack_top to tcb_ptr->sp_save_ptr */
  swi r1, r5, 0
  
  /* Reuse r15 for initial msr */
  lwi r15, r1, AC_MSR
  andi r15, r15, MSR_DEFAULT_MASK  
  ori r15, r15, MSR_IE
  swi r15, r1, AC_MSR
  swi r7, r1, AC_R15 /*R15 - entry point*/
  swi r8, r1, AC_R5 /*R5 - entry_param*/
  
  /* Restore the original stack */
  addk r1, r19, r0 
  
  lwi r15, r1, 0
  lwi r19, r1, 4
  rtsd r15, 8
  addik r1, r1, 8

/**
 * \b archFirstThreadRestore
 *
 * Architecture-specific function to restore and start the first thread.
 * This is called by atomOSStart() when the OS is starting.
 *
 * This function will be largely similar to the latter half of
 * archContextSwitch(). Its job is to restore the context for the
 * first thread, and finally enable interrupts.
 *
 * It expects to see the context saved in the same way as if the
 * thread has been previously scheduled out, and had its context
 * saved. That is, archThreadContextInit() will have been called
 * first (via atomThreadCreate()) to create a "fake" context save
 * area, containing the relevant register-save values for a thread
 * restore.
 *
 * Note that you can create more than one thread before starting
 * the OS - only one thread is restored using this function, so
 * all other threads are actually restored by archContextSwitch().
 * This is another reminder that the initial context set up by
 * archThreadContextInit() must look the same whether restored by
 * archFirstThreadRestore() or archContextSwitch().
 *
 * @param[in] new_tcb_ptr Pointer to the thread being scheduled in
 *
 * @return None
 *
 * void archFirstThreadRestore (ATOM_TCB *new_tcb_ptr)
 */
.global archFirstThreadRestore
archFirstThreadRestore:
  swi r5, r1, 4
  addik r1,r1,-8
  swi r19, r1, 4
  swi r15, r1, 0

  /* R5 - new_tcb_ptr */

  /* Load the stack_top of new_tcb_ptr->sp_save_ptr */
  lwi r1, r5, 0

  /* Load context off stack */
  LOAD_CONTEXT

archFirstThreadRestoreTrap:
  /* Should never get here */
  la r15, r0, archFirstThreadRestoreTrap
  bra r15
  nop
  
/*
 * \b archContextSwitch
 *
 * Architecture-specific context switch routine.
 *
 * Note that interrupts are always locked out when this routine is
 * called. For cooperative switches, the scheduler will have entered
 * a critical region. For preemptions (called from an ISR), the
 * ISR will have disabled interrupts on entry.
 *
 * @param[in] old_tcb_ptr Pointer to the thread being scheduled out
 * @param[in] new_tcb_ptr Pointer to the thread being scheduled in
 *
 * @return None
 *
 * void archContextSwitch (ATOM_TCB *old_tcb_ptr, ATOM_TCB *new_tcb_ptr)
 */
.global archContextSwitch
archContextSwitch:
  swi r5, r1, 4
  swi r6, r1, 8
  addik r1,r1,-8
  swi r19, r1, 4
  swi r15, r1, 0
  
  /* R5 - old_tcb_ptr */
  /* R6 - new_tcb_ptr */
  
  /* Set the return of our context to be past the context switch */
  la r15, r0, archContextSwitchReturn

  /* Save context to stack */
  STORE_CONTEXT
  
  /* Store the stack_top to old_tcb_ptr->sp_save_ptr */
  swi r1, r5, 0
  
  /* Load the stack_top of new_tcb_ptr->sp_save_ptr */
  lwi r1, r6, 0
  
  /* Load context off stack */
  LOAD_CONTEXT
  
archContextSwitchReturn:
  lwi r19, r1, 4
  lwi r15, r1, 0
  rtsd r15, 8
  addik r1, r1, 8
