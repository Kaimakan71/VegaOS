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

#include <amd64/idt.h>
#include <sys/bug.h>

#if defined(__x86_64__)

#define IDT_TRAP_GATE_FLAGS 0x8F
#define IDT_INT_GATE_FLAGS  0x8E
#define IDT_INT_GATE_USER   0xEE

static struct idt_entry idt[256];
static struct idtr idtr = {
  .limit = sizeof(struct idt_entry) * 256 - 1,
  .offset = (uintptr_t)&idt[0]
};

static void set_desc(uint8_t vector, uintptr_t isr, uint8_t flags)
{
  struct idt_entry *entry = &idt[vector];
  entry->offset_low = (uint16_t)isr;
  entry->offset_mid = (uint16_t)(isr >> 16);
  entry->offset_high = (uint32_t)(isr >> 32);
  entry->reserved = 0;
  entry->zero = 0;
  entry->ist = 0;
  entry->zero1 = 0;
  entry->cs = 0x28;
  entry->dpl = 3;
  entry->p = 1;
  entry->type = flags;
}

void idt_load(void)
{
  __asm("lidt %0" :: "m" (idtr));
}

void register_exception(uint8_t vector, uintptr_t isr)
{
  set_desc(vector, isr, IDT_TRAP_GATE_FLAGS);
}

#endif    /* __x86_64__ */
