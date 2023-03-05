/* author: Ian Marco Moffett */

#include <string.h>

void *memset(void *ptr, int val, size_t length)
{
  for (size_t i = 0; i < length; ++i)
  {
    ((uint8_t *)ptr)[i] = val;
  }

  return ptr;
}
