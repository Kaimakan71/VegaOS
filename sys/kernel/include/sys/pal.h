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

#ifndef _SYS_PAL_H_
#define _SYS_PAL_H_

#include <sys/cdefs.h>

#if defined(__x86_64__)

/*
 *  halt() only halts the current processor
 *  and it will continue if an interrupt
 *  fires. To fully halt the CPU ensure
 *  you use global_irq_disable() first.
 */

# define halt()                  __asm("hlt")

/*
 *  Prevents the processor from responding
 *  to external interrupts.
 *
 *  NOTE: This can cause a significant performance
 *        impact, use judiciously.
 */

# define global_irq_disable()    __asm("cli")

/*
 *  Allows the processor to respond
 *  to interrupts.
 */

# define global_irq_enable()     __asm("sti")

#elif defined(__aarch64__)
void global_irq_disable(void);
void global_irq_enable(void);

# define halt()                  __asm("0: wfi; b 0")
# define isb()                   __asm("isb")
# define dmb()                   __asm("dmb")

#endif    /* __x86_64__  */
#endif    /* _SYS_PAL_H_ */
