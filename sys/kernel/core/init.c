/* author: Ian Marco Moffett */

#include <sys/cdefs.h>
#include <sys/pal.h>
#include <sys/printk.h>
#include <sys/module.h>
#include <sys/types.h>
#include <mm/pmm.h>

#if defined(__x86_64__)
# include <amd64/exceptions.h>
# include <amd64/idt.h>
#elif defined(__aarch64__)
# include <aarch64/exceptions.h>
# include <aarch64/mmio.h>
#endif

MODULE("kinit");

#if defined(__aarch64__)
static void aarch64_init(void)
{
  exceptions_init();
  aarch64_mmio_init();
}
#elif defined(__x86_64__)
static void amd64_init(void)
{
  idt_load();
  exceptions_init();
}
#endif

__dead void _start(void)
{
  printk("VegaOS v%s - Copyright (c) 2023 Ian Marco Moffett\n",
         VEGA_VERSION
  );

  pmm_init();
#if defined(__x86_64__)
  amd64_init();
#elif defined(__aarch64__)
  aarch64_init();
#endif

  for (;;)
  {
    halt();
  }
}
