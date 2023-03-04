/* author: Ian Marco Moffett */

#include <sys/panic.h>
#include <sys/printk.h>
#include <sys/pal.h>

void panic(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  printk(__KERN_PANIC);
  vprintk(fmt, ap);
  va_end(ap);
  
  /* TODO: Halt all processors on SMP */
  global_irq_disable();
  halt();
}
