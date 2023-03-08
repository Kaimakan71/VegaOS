/* author: Ian Marco Moffett */

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
