/* author: Ian Marco Moffett */

#include <sys/cdefs.h>
#include <sys/pal.h>
#include <sys/printk.h>
#include <sys/module.h>
#include <sys/types.h>
#include <mm/pmm.h>
#include <mm/vmm.h>

#if defined(__x86_64__)
# include <amd64/exceptions.h>
# include <amd64/idt.h>
#elif defined(__aarch64__)
# include <aarch64/exceptions.h>
#endif

MODULE("kinit");

#if defined(__aarch64__)
static void aarch64_init(void)
{
}
#elif defined(__x86_64__)
static void amd64_init(void)
{
  idt_load();
  exceptions_init();
}
#endif

#if defined(__aarch64__)
static const char *get_board(void)
{
  uint32_t reg;
  asm volatile ("mrs %x0, midr_el1" : "=r" (reg));
  
  switch ((reg >> 4) & 0xFFF) {
    case 0xB76:
      return "Raspberry Pi 1";
    case 0xC07: 
      return "Raspberry Pi 2";
    case 0xD03:
      return "Raspberry Pi 3";
    case 0xD08: 
      return "Raspberry Pi 4";
    default:
      return "Unknown";
  }

}
#endif

__dead void _start(void)
{
  printk("VegaOS v%s - Copyright (c) 2023 Ian Marco Moffett\n",
         VEGA_VERSION
  );

#if defined(__x86_64__)
  amd64_init();
#elif defined(__aarch64__)
  aarch64_init();
  kinfo("Board: %s\n", get_board());
#endif

  pmm_init();
  vmm_init();
  exceptions_init();

  for (;;)
  {
    halt();
  }
}
