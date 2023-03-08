/* author: Ian Marco Moffett */

#include <aarch64/mm/mmu.h>
#include <aarch64/cpu.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <sys/module.h>
#include <sys/printk.h>
#include <sys/panic.h>
#include <sys/kaddr.h>
#include <sys/math.h>
#include <sys/limine.h>
#include <string.h>

MODULE("mmu");

#if defined(__aarch64__)

static volatile struct limine_kernel_address_request kaddr_req = {
  .id = LIMINE_KERNEL_ADDRESS_REQUEST,
  .revision = 0
};

static struct pagemap kpagemap;
static uintptr_t *next_level(uintptr_t *table, size_t index, uint8_t alloc)
{
  if ((table[index] & PTE_P) == 0)
  {
    if (!alloc)
    {
      return NULL;
    }

    table[index] = pmm_alloc(1) | PTE_P | PTE_TBL;
  }
  
  /* Mask off the offset by ANDing PA by ~(0xFFF) */
  return (uintptr_t *)((table[index] & ~(0xFFF)) + VMM_HIGHER_HALF);
}

#if 0
#endif

void aarch64_mmu_map_page(struct pagemap *p, uintptr_t virt, uintptr_t phys,
                          size_t flags, mmu_pagesize_t pagesize)
{
  size_t index0 = (virt >> 39) & 0x1FF;
  size_t index1 = (virt >> 30) & 0x1FF;
  size_t index2 = (virt >> 21) & 0x1FF;
  size_t index3 = (virt >> 12) & 0x1FF;
  size_t pte_flags = PTE_P;

  if ((flags & VMM_WRITABLE) == 0)
  {
    pte_flags |= PTE_RO;
  }

  if ((flags & VMM_EXEC) == 0)
  {
    pte_flags |= PTE_NX;
  }

  if ((flags & VMM_GLOBAL) == 0)
  {
    pte_flags |= PTE_NG;
  }

  if ((flags & VMM_USER) != 0)
  {
    pte_flags |= PTE_U;
  }
  
  uint8_t top_half = (virt & (1ULL << 63)) != 0;
  uintptr_t *root = (uintptr_t *)(p->ttbr[top_half] + VMM_HIGHER_HALF);

  root = next_level(root, index0, 1);
  
  if (pagesize == MMU_PAGESIZE_1G)
  {
    root[index1] = (pte_flags | phys) & ~(PTE_TBL);
    return;
  }

  root = next_level(root, index1, 1);
  
  if (pagesize == MMU_PAGESIZE_2M)
  {
    /* Huge page mapping */
    root[index2] = (pte_flags | phys) & ~(PTE_TBL);
    return;
  }

  root = next_level(root, index2, 1);
  
  /* Emulate x86's SMAP & SMEP using NX bits */
  if (top_half)
  {
    pte_flags |= PTE_UXN;
  }
  else
  {
    pte_flags |= PTE_PXN;
  }

  root[index3] = pte_flags | phys | PTE_ISH;
}

void aarch64_mmu_init(void)
{
  size_t id_mmfr0 = cpu_read_sysreg(id_aa64mmfr0_el1);
  uint64_t fb_attr = ((cpu_read_sysreg(mair_el1) >> 8) & 0xFF);;
  uint8_t pa_bits = id_mmfr0 & 0xF;

  if (((id_mmfr0 >> 28) & 0xF) == 0xF)
  {
    panic("4KB translation granule is not supported!\n");
  }

  if ((id_mmfr0 & 0xF) < 1)
  {
    panic("48-bit physical addresses not supported!\n");
  }

  if (id_mmfr0 & (1 << 5) == 0)
  {
    panic("16-bit ASIDs not supported!\n");
  }
  
  kpagemap.ttbr[0] = pmm_alloc(1);
  kpagemap.ttbr[1] = pmm_alloc(1);
  memset((void*)(kpagemap.ttbr[0] + VMM_HIGHER_HALF), 0, 4096);
  memset((void*)(kpagemap.ttbr[1] + VMM_HIGHER_HALF), 0, 4096);
  kinfo("Allocated physical memory for TTBR0 at %x\n", kpagemap.ttbr[0]);

  size_t tcr =
    (1 << 4)       |             /* T0SZ=16 */
    (16 << 16)     |             /* T1SZ=16 */
    (1 << 8)       |             /* TTBR0 Inner WB RW-Allocate */
    (1 << 10)      |             /* TTBR0 Outer WB RW-Allocate */
    (2 << 12)      |             /* TTBR0 Inner shareable */
    (1 << 24)      |             /* TTBR1 Inner WB RW-Allocate */
    (1 << 26)      |             /* TTBR1 Outer WB RW-Allocate */
    (2 << 28)      |             /* TTBR1 Inner shareable */
    (2ULL << 30)   |             /* TTBR1 4K granule */
    (1ULL << 36)   |             /* 16-bit ASIDs */
    ((size_t)pa_bits << 32);     /* 48-bit intermediate address */

  size_t mair =
    (0xFF << 0)    |     /* Normal, write-back, rw-alloc, non-transient */
    (fb_attr << 8) |     /* Framebuffer memory (not always write-combining) */
    (0x00 << 16)   |     /* Device (uncachable) */
    (0x04 << 24);        /* Normal, (uncachable) */
  
  cpu_write_sysreg(mair_el1, mair);
  cpu_write_sysreg(tcr_el1, tcr);
 
  /*
   *  Map 2G of memory
   *  so stuff like the 
   *  EVT can be accessed.
   */

  for (uintptr_t i = MMU_1G; i < MMU_1G*2; i += MMU_1G)
  {
    aarch64_mmu_map_page(
        &kpagemap,
        i+VMM_HIGHER_HALF,
        i,
        VMM_WRITABLE,
        MMU_PAGESIZE_1G);
  }

  cpu_write_sysreg(ttbr0_el1, kpagemap.ttbr[0]);  /* Set the lower half */
  kinfo("Wrote TCR, MAIR and TTBR0_EL1\n");
}

#endif
