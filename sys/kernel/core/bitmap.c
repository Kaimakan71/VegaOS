/* author: Ian Marco Moffett */

#include <sys/bitmap.h>

void bitmap_set_bit(bitmap_t bitmap, size_t bit)
{
  bitmap[bit / 8] |= (1 << (bit % 8));
}

void bitmap_unset_bit(bitmap_t bitmap, size_t bit)
{
  bitmap[bit / 8] &= ~(1 << (bit % 8));
}

uint8_t bitmap_test_bit(bitmap_t bitmap, size_t bit)
{
  return (bitmap[bit / 8] & (1 << (bit % 8))) >> bit;
}
