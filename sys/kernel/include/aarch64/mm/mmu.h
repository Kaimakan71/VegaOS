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
#define MMU_2MB 0x200000
#define MMU_1G  0x40000000ULL

#if defined(__aarch64__)
typedef enum
{
  MMU_PAGESIZE_4K,
  MMU_PAGESIZE_2M,
  MMU_PAGESIZE_1G
} mmu_pagesize_t;

struct pagemap
{
  uintptr_t ttbr[2];
  uint16_t asid;
};
#endif    /* __aarch64__ */

void aarch64_mmu_init(void);


void aarch64_mmu_map_page(struct pagemap *p, uintptr_t virt, uintptr_t phys,
                          size_t flags, mmu_pagesize_t pagesize);

#endif
