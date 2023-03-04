/* author: Ian Marco Moffett */

#include <sys/cdefs.h>
#include <sys/pal.h>
#include <sys/printk.h>
#include <sys/module.h>

#if defined(__x86_64__)
#include <amd64/exceptions.h>
#include <amd64/idt.h>
#endif

#define VEGA_COPYRIGHT "Copyright (c) 2023 Ian Marco Moffett"

MODULE("kernel_init");

__dead void _start(void)
{
  printk("VegaOS v%s - %s\n\n", VEGA_VERSION, VEGA_COPYRIGHT);

#if defined(__x86_64__)
  idt_load();
  exceptions_init();
#endif

  for (;;)
  {
    halt();
  }
}
