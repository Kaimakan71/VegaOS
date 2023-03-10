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

#include <dev/irqchip/bcm2835.h>
#include <sys/cdefs.h>
#include <sys/module.h>
#include <sys/printk.h>
#include <mm/vmm.h>
#include <mm/pmm.h>

MODULE("bcm2835");

static uintptr_t mmio_virt = 0;
static uintptr_t get_mmio_phys(void)
{
  uint32_t reg;
  __asm("mrs %x0, midr_el1" : "=r" (reg));

  switch ((reg >> 4) & 0xFFF)
  {
    case 0xB76:
      return 0x20000000;
    case 0xC07: 
      return 0x3F000000;
    case 0xD03:
      return 0x3F000000;
    case 0xD08: 
      return 0xFE000000;
    default:
      return 0x20000000;
  }
}

void bcm2835_init(void)
{ 
  /*
   *  The pmm_alloc() call
   *  is used to leak a
   *  single pageframe
   *  for use as a virtual address.
   */
  
  mmio_virt = pmm_alloc(1);
  vmm_map_page(
      vmm_get_vas(),
      mmio_virt,
      0xFE000000,
      VMM_WRITABLE |
      VMM_GLOBAL,
      PAGESIZE_4K
  );

  kinfo("MMIO virtual base %x\n", mmio_virt);
}
