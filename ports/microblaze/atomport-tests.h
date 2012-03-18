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

#ifndef __ATOM_PORT_TESTS_H
#define __ATOM_PORT_TESTS_H

/* Include Atomthreads kernel API */
#include "atom.h"

/* Prerequisite include for ATOMLOG() macro (via printf) */
#include <stdio.h>

/* Logger macro for viewing test results */
/* Xilinx documentation states that printf is not thread-safe, so wrap 
 * it around a critical section for safety.
 */
//#define ATOMLOG printf
//#define ATOMLOG(format, ...) {CRITICAL_STORE; CRITICAL_START(); printf(format, ##__VA_ARGS__); CRITICAL_END();}
#define ATOMLOG(format, ...) {CRITICAL_STORE; CRITICAL_START(); print(format); CRITICAL_END();}

/*
 * String location macro: for platforms which need to place strings in
 * alternative locations, e.g. on avr-gcc strings can be placed in
 * program space, saving SRAM. On most platforms this can expand to
 * empty.
 */
#define _STR(x)     x

/* Default thread stack size (in bytes) */
#define TEST_THREAD_STACK_SIZE      1024

/* Uncomment to enable logging of stack usage to UART */
#define TESTS_LOG_STACK_USAGE


#endif /* __ATOM_PORT_TESTS_H */

