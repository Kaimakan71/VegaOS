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

#include <aarch64/mm/mmu.h>
#include <aarch64/cpu.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <sys/module.h>
#include <sys/math.h>
#include <sys/printk.h>
#include <sys/cdefs.h>
#include <sys/panic.h>

MODULE("mmu");

#if defined(__aarch64__)

static struct aarch64_pagemap pagemap;

static inline void invalidate_page(uintptr_t vaddr)
{
  uintptr_t page_number = vaddr >> 12;
  __asm__ __volatile__ (
    "dsb ish\n\t"       /* Ensure that all memory accesses have completed */
    "dc isw, %0\n\t"    /* Invalidate the page using the ISW operation code */
    "dsb ish\n\t"       /* Ensure completion of cache invalidation */
    "isb\n\t"           /* Ensure instruction stream synchronization */
    :
    : "r" (page_number)
    : "memory"
  );
}

static uintptr_t next_level(uintptr_t level_phys, size_t index, uint8_t alloc)
{
  uintptr_t *level_virt = (uintptr_t *)(level_phys + VMM_HIGHER_HALF);

  if (level_virt[index] & PTE_P)
  {
    return level_virt[index] & PTE_ADDR_MASK;
  }

  if (!alloc)
  {
    return 0;
  }
  
  uintptr_t next = pmm_alloc(1);
  
  if (next == 0)
  {
    panic("Failed to allocate pageframe for next level\n");
  }

  level_virt[index] = next  |
                      PTE_P |
                      PTE_TBL;
  return alloc;
}

/*
 *  Walks the translation table by
 *  extracting the first-level lookup
 *  from TTBRn (where n is 1 for
 *  higher half virtual addresses
 *  and 0 for lower half virtual addresses),
 *
 *  and doing the following:
 *  
 *
 *  FIRST_LEVEL -> SECOND_LEVEL -------+
 *  THIRD_LEVEL -> PHYSICAL_ADDR       |
 *  ^                                  |
 *  |                                  |
 *  +----------------------------------+
 *
 *  NOTE: Assumes 48-bit physical addresses,
 *        4-level paging and 4K granule.
 *
 */

uintptr_t aarch64_translate_vaddr(struct aarch64_pagemap p, uintptr_t vaddr)
{
  uint8_t ttbr_index = vaddr >= VMM_HIGHER_HALF;

  size_t level0_index = (vaddr >> 39) & 0x1FF;
  size_t level1_index = (vaddr >> 30) & 0x1FF;
  size_t level2_index = (vaddr >> 21) & 0x1FF;
  size_t level3_index = (vaddr >> 12) & 0x1FF; 
  
  uintptr_t lookup_level_0 = p.ttbr[ttbr_index] & PTE_ADDR_MASK;  
  uintptr_t current_level = next_level(lookup_level_0, level0_index, 0);

  if (current_level == 0)
  {
    return 0;
  }

  current_level = next_level(current_level, level1_index, 0);
  
  if (current_level == 0)
  {
    return 0;
  }

  current_level = next_level(current_level, level2_index, 0);
  
  if (current_level == 0)
  {
    return 0;
  }

  return next_level(current_level, level3_index, 0);
}


void aarch64_map_page(struct aarch64_pagemap p, uintptr_t vaddr,
                      uintptr_t phys, size_t flags,
                      pagesize_t pagesize)
{
  uint8_t ttbr_index = vaddr >= VMM_HIGHER_HALF;

  size_t level0_index = (vaddr >> 39) & 0x1FF;
  size_t level1_index = (vaddr >> 30) & 0x1FF;
  size_t level2_index = (vaddr >> 21) & 0x1FF;
  size_t level3_index = (vaddr >> 12) & 0x1FF; 

  if ((flags & VMM_WRITABLE) == 0)
  {
    flags |= PTE_RO;
  }

  if ((flags & VMM_EXEC) == 0)
  {
    flags |= PTE_NX;
  }

  if ((flags & VMM_USER) != 0)
  {
    flags |= PTE_U;
  }

  if ((flags & VMM_GLOBAL) == 0)
  {
    flags |= PTE_NG;
  }

  /* 
   *  Emulate SMEP with NX bits
   *  for security purposes.
   *
   *  If ttbr_index is 1, then
   *  the virtual address
   *  is on the higher half.
   *
   *  If the ttbr_index is 0,
   *  then the virtual address
   *  is on the lower half.
   */
  if (ttbr_index == 1)
  {
    flags |= PTE_UXN;
  }
  else if (ttbr_index == 0)
  {
    flags |= PTE_PXN;
  }

  uintptr_t lookup_level_0 = p.ttbr[ttbr_index] & PTE_ADDR_MASK;  
  uintptr_t current_level = next_level(lookup_level_0, level0_index, 1);

  current_level = next_level(current_level, level1_index, 1); 
  current_level = next_level(current_level, level2_index, 1);
  
  uintptr_t *pte = (uintptr_t *)(current_level + VMM_HIGHER_HALF);
  pte[level3_index] = PTE_P   |
                      PTE_AF  |
                      PTE_TBL |
                      phys    |
                      flags;

  invalidate_page(vaddr);
}


void aarch64_unmap_page(struct aarch64_pagemap p, uintptr_t vaddr)
{
  uint8_t ttbr_index = vaddr >= VMM_HIGHER_HALF;
  size_t level0_index = (vaddr >> 39) & 0x1FF;
  size_t level1_index = (vaddr >> 30) & 0x1FF;
  size_t level2_index = (vaddr >> 21) & 0x1FF;
  size_t level3_index = (vaddr >> 12) & 0x1FF; 
  
  uintptr_t lookup_level_0 = p.ttbr[ttbr_index] & PTE_ADDR_MASK;  
  uintptr_t current_level = next_level(lookup_level_0, level0_index, 0);

  if (current_level == 0)
  {
    return;
  }

  current_level = next_level(current_level, level1_index, 1);
  
  if (current_level == 0)
  {
    return;
  }

  current_level = next_level(current_level, level2_index, 1);
  if (current_level == 0)
  {
    return;
  }
  
  uintptr_t *pte = (uintptr_t *)(current_level + VMM_HIGHER_HALF);
  pte[level3_index] = 0;
  invalidate_page(vaddr);
}

struct aarch64_pagemap aarch64_get_pagemap(void)
{
  struct aarch64_pagemap pagemap;
  pagemap.ttbr[0] = cpu_read_sysreg(ttbr0_el1);
  pagemap.ttbr[1] = cpu_read_sysreg(ttbr1_el1);
  pagemap.asid = cpu_read_sysreg(contextidr_el1) & 0xFFFF;
  return pagemap;
}

void aarch64_mmu_init(void)
{
  size_t fb_attr  = (cpu_read_sysreg(mair_el1) >> 8) & 0xFF;
  size_t id_mmfr0 = cpu_read_sysreg(id_aa64mmfr0_el1);
  size_t pa_bits  = id_mmfr0 & 0xF;
  
  /* 
   * Systems like QEMU map framebuffer as 0xFF.
   * Override it.
   */
  if (fb_attr == 0xFF)
  {
    fb_attr = 0xC;
  }

  size_t mair =
    (fb_attr << 8)     | /* Framebuffer (not always write-combining) */
    (0xFF << 0)        | /* Normal (Write-back, RW allocate, non-transient) */
    (0x00 << 16)       | /* Device (nGnRnE) */
    (0x04 << 24);        /* Normal Uncachable (device) */

  size_t tcr =
    (0x10 << 0)    |      /* T0SZ=16 */
    (0x10 << 16)   |      /* T1SZ=16 */
    (0x02 << 30)   |      /* 4K granule */
    (0x01 << 8)    |      /* TTBR0 Inner WB RW-Allocate */
    (0x01 << 10)   |      /* TTBR0 Outer WB RW-Allocate */
    (0x02 << 12)   |      /* TTBR0 Inner shareable */
    (0x01 << 24)   |      /* TTBR1 Inner WB RW-Allocate */
    (0x01 << 26)   |      /* TTBR1 Outer WB RW-Allocate */
    (0x02 << 28)   |      /* TTBR1 Inner shareable */
    (0x01UL << 36) |      /* Use 16-bit ASIDs */
    (pa_bits << 32);      /* 48-bit intermediate address */
  
  cpu_write_sysreg(mair_el1, mair);
  cpu_write_sysreg(tcr_el1, tcr);

  kinfo("Wrote MAIR and TCR for EL1\n"); 
  pagemap = aarch64_get_pagemap();
}

#endif
