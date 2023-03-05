/* author: Ian Marco Moffett */

#ifndef _SYS_BITMAP_H_
#define _SYS_BITMAP_H_

#include <sys/types.h>

typedef uint8_t *bitmap_t;

void bitmap_set_bit(bitmap_t bitmap, size_t bit);
void bitmap_unset_bit(bitmap_t bitmap, size_t bit);
uint8_t bitmap_test_bit(bitmap_t bitmap, size_t bit);

#endif
