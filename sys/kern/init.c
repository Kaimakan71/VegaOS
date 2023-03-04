/* author: Ian Marco Moffett */

#include <sys/cdefs.h>
#include <sys/pal.h>

__dead void _start(void)
{
  for (;;)
  {
    halt();
  }
}
