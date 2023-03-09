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

#include <sys/cdefs.h>
#include <sys/pal.h>
#include <sys/printk.h>
#include <sys/module.h>
#include <sys/types.h>
#include <mm/pmm.h>

#if defined(__x86_64__)
# include <amd64/exceptions.h>
# include <amd64/idt.h>
#elif defined(__aarch64__)
# include <aarch64/exceptions.h>
#endif

MODULE("kinit");

#if defined(__aarch64__)
static void aarch64_init(void)
{
  exceptions_init();
}
#elif defined(__x86_64__)
static void amd64_init(void)
{
  idt_load();
  exceptions_init();
}
#endif

__dead void _start(void)
{
  printk("VegaOS v%s - Copyright (c) 2023 Ian Marco Moffett\n",
         VEGA_VERSION
  );

  pmm_init();
#if defined(__x86_64__)
  amd64_init();
#elif defined(__aarch64__)
  aarch64_init();
#endif

  for (;;)
  {
    halt();
  }
}
