/* author: Ian Marco Moffett */

#include <sys/cdefs.h>
#include <sys/pal.h>
#include <pty/core.h>

__dead void _start(void)
{
  pty_putstr("Hello, World!\n", 0xFFFFFF);

  for (;;)
  {
    halt();
  }
}
