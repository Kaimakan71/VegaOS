/*
 * Copyright (c) 2023 Ian Marco Moffett and the VegaOS team.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of VegaOS nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <amd64/exceptions.h>
#include <amd64/idt.h>
#include <sys/panic.h>

#if defined(__x86_64__)

#define MAKE_ISR(vector, name)               \
  static void exception_##vector(void* sf)   \
  {                                          \
    panic("%s (vector=%x)\n", name, vector); \
  }

#define REGISTER_EXCEPTION(vector) \
  register_exception(vector, (uintptr_t)exception_##vector)

MAKE_ISR(0x0, "Divide error")
MAKE_ISR(0x1, "Debug exception")
MAKE_ISR(0x3, "Breakpoint exception")    /* TODO: Sleep instead of halting */
MAKE_ISR(0x4, "Overflow")
MAKE_ISR(0x5, "BOUND range exceeded")
MAKE_ISR(0x6, "Invalid opcode")
MAKE_ISR(0x7, "Device not available")
MAKE_ISR(0x8, "Double fault")
MAKE_ISR(0xA, "Invalid TSS")
MAKE_ISR(0xB, "Segment not present")
MAKE_ISR(0xC, "Stack segment fault")
MAKE_ISR(0xD, "General protection fault")
MAKE_ISR(0xE, "Page fault")

void exceptions_init(void)
{
  REGISTER_EXCEPTION(0x0);
  REGISTER_EXCEPTION(0x1);
  REGISTER_EXCEPTION(0x3);
  REGISTER_EXCEPTION(0x4);
  REGISTER_EXCEPTION(0x5);
  REGISTER_EXCEPTION(0x6);
  REGISTER_EXCEPTION(0x7);
  REGISTER_EXCEPTION(0x8);
  REGISTER_EXCEPTION(0xA);
  REGISTER_EXCEPTION(0xB);
  REGISTER_EXCEPTION(0xC);
  REGISTER_EXCEPTION(0xD);
  REGISTER_EXCEPTION(0xE);
}

#endif    /* __x86_64__ */
