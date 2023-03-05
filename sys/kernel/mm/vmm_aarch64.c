/*
 *  description: Virtual memory manager for AARCH64.
 *  author: Ian Marco Moffett.
 */

#include <mm/vmm.h>
#include <mm/pmm.h>
#include <sys/module.h>
#include <sys/printk.h>
#include <sys/cdefs.h>

#if defined(__aarch64__)
MODULE("vmm");

#define GET_PML1_IDX(virt) ((virt >> 12) & 0x1FF)
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
#define PT_ADDR_MASK    ((size_t)0x0000FFFFFFFFF000)

static uintptr_t kernel_vas = 0;

volatile struct limine_hhdm_request g_hhdm_request = {
  .id = LIMINE_HHDM_REQUEST,
  .revision = 0
};

static uintptr_t get_next_level(uintptr_t pagemap_phys, size_t idx,
                                uint8_t alloc)
{
  uintptr_t *pagemap = (uintptr_t *)(pagemap_phys + VMM_HIGHER_HALF);

  if (pagemap[idx] & PT_FLAG_VALID)
  {
    uintptr_t phys = pagemap[idx] & PT_ADDR_MASK; 
    return phys;
  }

  if (!alloc)
  {
    return 0;
  }

  uintptr_t next_level = pmm_alloc(1);
  pagemap[idx] = next_level  | PT_FLAG_VALID
                             | PT_FLAG_USER
                             | PT_FLAG_TABLE;
  return next_level;
}

static uintptr_t get_pml1(uintptr_t pagemap_phys, uintptr_t virt,
                          uint8_t alloc)
{

  size_t pml4_index = (virt >> 39) & 0x1FF;
  size_t pml3_index = (virt >> 30) & 0x1FF;
  size_t pml2_index = (virt >> 21) & 0x1FF;

  uintptr_t pml3 = get_next_level(pagemap_phys, pml4_index, alloc);

  if (pml3 == 0)
  {
    return 0;
  }

  uintptr_t pml2 = get_next_level(pml3, pml3_index, alloc);

  if (pml2 == 0)
  {
    return 0;
  }
  
  return get_next_level(pml2, pml2_index, alloc);
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
                        | PT_FLAG_USER
                        | PT_FLAG_INNER_SH
                        | PT_FLAG_WB
                        | PT_FLAG_ACCESS;

  if (!(flags & VMM_WRITABLE))
  {
    actual_flags |= PT_FLAG_READONLY;
  }

  if (flags & VMM_NX)
  {
    actual_flags |= PT_FLAG_XN;
  }
  
  uintptr_t pml1 = get_pml1(pagemap_phys, virt, 1);
  ((uintptr_t*)pml1)[GET_PML1_IDX(virt)] = phys | actual_flags;
}

#endif    /* __aarch64__ */
