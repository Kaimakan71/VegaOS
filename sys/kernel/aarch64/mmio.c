/* author: Ian Marco Moffett */

#include <aarch64/mmio.h>
#include <mm/vmm.h>
#include <mm/pmm.h>

#if defined(__aarch64__)

uintptr_t aarch64_get_mmio_base(void)
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


void aarch64_mmio_init(void)
{
  uintptr_t mmio_base = aarch64_get_mmio_base();

  vmm_map_page(vmm_get_vas(),
               mmio_base,
               mmio_base,
               VMM_PRESENT | VMM_WRITABLE
  );
}

#endif
