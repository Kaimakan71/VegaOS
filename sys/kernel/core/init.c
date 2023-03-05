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
  /* TODO */
#endif

#define VEGA_COPYRIGHT "Copyright (c) 2023 Ian Marco Moffett"

MODULE("kernel_init");

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

__dead void _start(void)
{
  printk("VegaOS v%s - %s\n\n", VEGA_VERSION, VEGA_COPYRIGHT);

#if defined(__x86_64__)
  amd64_init();
#elif defined(__aarch64__)
  aarch64_init();
#endif

  pmm_init();

  for (;;)
  {
    halt();
  }
}
