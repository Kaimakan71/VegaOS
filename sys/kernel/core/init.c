/* author: Ian Marco Moffett */

#include <sys/cdefs.h>
#include <sys/pal.h>
#include <sys/printk.h>
#include <sys/module.h>
#include <amd64/exceptions.h>
#include <amd64/idt.h>

#define VEGA_COPYRIGHT "Copyright (c) 2023 Ian Marco Moffett"

MODULE("kernel_init");

__dead void _start(void)
{
  printk("VegaOS v%s - %s\n\n", VEGA_VERSION, VEGA_COPYRIGHT);

  idt_load();
  exceptions_init();

  for (;;)
  {
    halt();
  }
}
