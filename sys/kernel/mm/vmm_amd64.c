/* author: Ian Marco Moffett */

#include <mm/vmm.h>
#include <mm/pmm.h>

#if defined(__x86_64__)

static uintptr_t get_next_level(uintptr_t pagemap_phys, size_t idx,
                                uint8_t alloc)
{
  uintptr_t *pagemap = (uintptr_t *)(pagemap_phys + VMM_HIGHER_HALF);

  if (pagemap[idx] & VMM_PRESENT)
  {
    uintptr_t phys = pagemap[idx] & PT_ADDR_MASK; 
    return phys;
  }

  if (!alloc)
  {
    return 0;
  }

  uintptr_t next_level = pmm_alloc(1);
                             | VMM_PRESENT
                             | VMM_WRITABLE
                             | VMM_USER;
  pagemap[idx] = next_level 
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
  uintptr_t pml1 = get_pml1(pagemap_phys, virt, 1);
  ((uintptr_t*)pml1)[GET_PML1_IDX(virt)] = phys
                                           | VMM_PRESENT
                                           | flags;
}

#endif
