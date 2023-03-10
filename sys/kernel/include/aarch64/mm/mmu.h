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

#ifndef _MM_MMU_H_
#define _MM_MMU_H_

#include <sys/types.h>
#include <mm/pagesize.h>

#define PTE_P   (1ULL << 0)
#define PTE_TBL (1ULL << 1)
#define PTE_U   (1ULL << 6)
#define PTE_RO  (1ULL << 7)
#define PTE_OSH (2ULL << 8)
#define PTE_ISH (3ULL << 8)
#define PTE_AF  (1ULL << 10)
#define PTE_NG  (1ULL << 11)
#define PTE_PXN (1ULL << 53)
#define PTE_UXN (1ULL << 54)
#define PTE_NX  (PTE_PXN | PTE_UXN)
#define MMU_2MB 0x200000ULL
#define MMU_1G  0x40000000ULL
#define PTE_ADDR_MASK 0x0000FFFFFFFFF000

struct aarch64_pagemap
{
  uintptr_t ttbr[2];
  uint16_t asid;
};

/*
 *  Initializes the MMU.
 */

void aarch64_mmu_init(void);

/*
 *  Walks the translation tables
 *  and returns the physical address if found.
 *
 *  Otherwise, returns 0.
 */

uintptr_t aarch64_translate_vaddr(struct aarch64_pagemap p, uintptr_t vaddr);

/*
 *  Maps a virtual address to a physical
 *  address.
 *
 *  p: Pagemap.
 *  vaddr: Virtual address.
 *  phys: Physical address.
 *  flags: PTE flags (uses flags like VMM_PRESENT, etc)
 *  pagesize: Size of the page (supports huge pages)
 *
 */

void aarch64_map_page(struct aarch64_pagemap p, uintptr_t vaddr,
                      uintptr_t phys, size_t flags,
                      pagesize_t pagesize);

/*
 *  Unmaps a virtual address.
 *
 *  @p: Pagemap.
 *  @vaddr: Virtual address.
 */

void aarch64_unmap_page(struct aarch64_pagemap p, uintptr_t vaddr);

/*
 *  Returns top-level pagemap.
 */

struct aarch64_pagemap aarch64_get_pagemap(void);

#endif
