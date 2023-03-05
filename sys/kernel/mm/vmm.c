/*
 *  description: Virtual memory manager.
 *  author: Ian Marco Moffett.
 */

#include <mm/vmm.h>
#include <mm/pmm.h>
#include <sys/module.h>
#include <sys/printk.h>
#include <sys/cdefs.h>

MODULE("vmm");

static uintptr_t* kernel_pml4 = NULL;

volatile struct limine_hhdm_request g_hhdm_request = {
  .id = LIMINE_HHDM_REQUEST,
  .revision = 0
};

/*
 *  Returns the top level
 *  translation table physical
 *  address.
 */

static uintptr_t get_toplevel_tt(void)
{
  uintptr_t phys = 0;

#if defined(__aarch64__)
  __asm("mrs %0, ttbr0_el1" : "=r" (phys));
#else
  __asm("mov %%cr3, %0" : "=r" (phys));
#endif
  
  return phys;
}

/*
 *  Sets the top level
 *  translation table physical
 *  address.
 */

static void set_toplevel_tt(uintptr_t phys)
{
#if defined(__aarch64__)
  __asm("msr ttbr0_el1, %0" :: "r" (phys));
#else
  __asm("mov %0, %%cr3" :: "r" (phys));
#endif
  
}

void vmm_init(void)
{
  /* Allocate memory for the kernel PML4 */
  kernel_pml4 = (uintptr_t*)(pmm_alloc(1) + VMM_HIGHER_HALF);
  
  /* Copy old top level translation table to new one */
  uintptr_t toplevel_tt_phys = get_toplevel_tt();
  
  for (size_t i = 0; i < 512; ++i)
  {
    ((uintptr_t*)kernel_pml4)[i] = ((uintptr_t*)toplevel_tt_phys)[i];
  }
  
  toplevel_tt_phys = (uintptr_t)kernel_pml4 - VMM_HIGHER_HALF;
  set_toplevel_tt(toplevel_tt_phys);

#if defined(__aarch64__)
  kinfo("Loaded TTBR0_EL1! (top_level_tt_phys=%x)\n", toplevel_tt_phys);
#else
  kinfo("Loaded CR3! (top_level_tt_phys=%x)\n", toplevel_tt_phys);
#endif
}
