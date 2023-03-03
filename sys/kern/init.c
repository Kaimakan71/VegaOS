/* author: Ian Marco Moffett */

#include <sys/cdefs.h>

__dead void _start(void)
{
  for (;;)
  {
    __asm("hlt");
  }
}
