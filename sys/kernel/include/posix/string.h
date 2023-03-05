/* author: Ian Marco Moffett */

#ifndef _POSIX_STRING_H_
#define _POSIX_STRING_H_

#include <sys/types.h>

void *memcpy(void *s1, const void *s2, size_t n);
void *memset(void *ptr, int val, size_t length);

#endif
