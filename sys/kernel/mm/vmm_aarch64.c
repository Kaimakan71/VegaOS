/*
 *  description: Virtual memory manager for AARCH64.
 *  author: Ian Marco Moffett.
 */

#include <mm/vmm.h>
#include <mm/pmm.h>
#include <sys/module.h>
#include <sys/printk.h>
#include <sys/cdefs.h>
#include <string.h>

#if defined(__aarch64__)
MODULE("vmm");

#define PT_4K_GRANULE    0x3
#define PT_FLAG_VALID    ((size_t)1 << 0)
#define PT_FLAG_TABLE    ((size_t)1 << 1)
#define PT_FLAG_4K_PAGE  ((size_t)1 << 1)
#define PT_FLAG_BLOCK    ((size_t)0 << 1)
#define PT_FLAG_USER     ((size_t)1 << 6)
#define PT_FLAG_READONLY ((size_t)1 << 7)
#define PT_FLAG_XN       ((size_t)1 << 54)
#define PT_FLAG_INNER_SH ((size_t)3 << 8)
#define PT_FLAG_ACCESS   ((size_t)1 << 10)
#define PT_FLAG_WB       ((size_t)0 << 2)

static uintptr_t next_level(uintptr_t pagemap_phys, size_t idx, uint8_t alloc)
{
  uintptr_t *pagemap = (uintptr_t *)(pagemap_phys + VMM_HIGHER_HALF);
  size_t entry = pagemap[idx];

  if ((entry & PT_FLAG_VALID) != 0)
  {
    return entry;
  }

  if (!alloc)
  {
    return 0;
  }

  uintptr_t next_level = pmm_alloc(1);
  memset((void*)next_level, 0, 4096);
  pagemap[idx] = next_level
                 | PT_FLAG_TABLE
                 | PT_FLAG_VALID
                 | PT_FLAG_USER
                 | PT_FLAG_INNER_SH
                 | PT_FLAG_ACCESS
                 | PT_FLAG_WB;

  return next_level;
}

void vmm_map_page(uintptr_t pagemap_phys, uintptr_t virt, uintptr_t phys,
                  size_t flags)
{
  /*
   *  actual_flags are actual flags that go
   *  into the translation table.
   *
   *  flags is an abstraction.
   */
  size_t actual_flags = PT_FLAG_VALID
                        | PT_FLAG_4K_PAGE
                        | PT_FLAG_INNER_SH
                        | PT_FLAG_ACCESS;

  if ((flags & VMM_WRITABLE) == 0)
  {
    actual_flags |= PT_FLAG_READONLY;
  }

  if ((flags & VMM_USER) != 0)
  {
    actual_flags |= PT_FLAG_USER;
  }

  size_t pml0_index = (virt >> 39) & 0x1FF;
  size_t pml1_index = (virt >> 30) & 0x1FF;
  size_t pml2_index = (virt >> 21) & 0x1FF;
  size_t pml3_index = (virt >> 12) & 0x1FF;

  uintptr_t pml1 = next_level(pagemap_phys, pml0_index, 1);
  uintptr_t pml2 = next_level(pml1, pml1_index, 1);
  uintptr_t pml3 = next_level(pml2, pml2_index, 1);

  uintptr_t *pml3_virt = (uintptr_t *)(pml3 + VMM_HIGHER_HALF);
  pml3_virt[pml3_index] = actual_flags | phys; 
}

#endif    /* __aarch64__ */
