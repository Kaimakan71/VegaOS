/*
 * Copyright (c) 2023 Ian Marco Moffett and the VegaOS team.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of VegaOS nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <mm/pmm.h>
#include <mm/vmm.h>
#include <sys/limine.h>
#include <sys/module.h>
#include <sys/printk.h>
#include <sys/math.h>
#include <sys/panic.h>
#include <sys/bitmap.h>
#include <string.h>

MODULE("pmm");

static struct limine_memmap_response *mmap_resp = NULL;
static bitmap_t bitmap = NULL;

/*
 *  There's going to be a gap
 *  where there's a *lot* of bits
 *  marked as not free, bitmap_start_bit
 *  points to the end of that gap which
 *  speeds up the search for a free pageframe
 *  by a ton.
 */

static size_t bitmap_start_bit = 0;

volatile struct limine_memmap_request mmap_req = {
  .id = LIMINE_MEMMAP_REQUEST,
  .revision = 0
};

static struct limine_memmap_entry *find_highest_mmap_entry(void)
{
  struct limine_memmap_entry *entry = NULL;

  for (size_t i = 0; i < mmap_resp->entry_count; ++i)
  {
    struct limine_memmap_entry *current_entry = mmap_resp->entries[i];
    if (current_entry->type != LIMINE_MEMMAP_USABLE)
    {
      continue;
    }

    if (entry == NULL) 
    {
      entry = current_entry;
    } 
    else if (current_entry->base > entry->base) 
    {
      entry = current_entry;
    }
  }

  return entry;
}

static size_t get_bitmap_size(void)
{
  struct limine_memmap_entry *highest_entry = find_highest_mmap_entry();
  
  if (highest_entry == NULL)    /* Sanity check */
  {
    panic("pmm: Could not find highest entry!\n");
  }

  size_t highest_address = highest_entry->base + highest_entry->length;
  size_t bitmap_size = ALIGN_UP((highest_address/0x1000)/8, 0x1000);
  return bitmap_size;
}

static void init_bitmap(void)
{
  size_t bitmap_size = get_bitmap_size();
  kinfo("Required bitmap size: %d bytes\n", bitmap_size);

  /* Find a memory segment for our bitmap */
  for (size_t i = 0; i < mmap_resp->entry_count; ++i)
  {
    struct limine_memmap_entry *entry = mmap_resp->entries[i];

    if (entry->type != LIMINE_MEMMAP_USABLE)
    {
      continue;
    }

    if (entry->length >= bitmap_size)
    {
      bitmap = (uint8_t *)(entry->base + VMM_HIGHER_HALF);
      memset(bitmap, 0xFF, bitmap_size);
      entry->length -= bitmap_size;
      entry->base += bitmap_size;    
      break;
    }
  }

  kinfo("Found space for bitmap at %x\n", bitmap);

  /* Setup bitmap based on memory map */
  for (size_t i = 0; i < mmap_resp->entry_count; ++i) 
  {
    struct limine_memmap_entry* entry = mmap_resp->entries[i];
    if (entry->type == LIMINE_MEMMAP_USABLE && entry->length >= bitmap_size) 
    {
      for (size_t j = 0; j < entry->length; j += 0x1000) 
      {
        bitmap_unset_bit(bitmap, (entry->base + j)/0x1000);

        if (bitmap_start_bit == 0)
        {
          bitmap_start_bit = (entry->base + j)/0x1000;
        }
      }
    }
  }
}


uintptr_t pmm_alloc(size_t frames)
{
  size_t bit = 0;
  size_t frames_found = 0;
  
  for (size_t current_bit = bitmap_start_bit; current_bit < get_bitmap_size()*8; ++current_bit)
  {
    if (!bitmap_test_bit(bitmap, current_bit))
    {
      if (bit == 0)
      {
        bit = current_bit;
      }

      frames_found++;

      if (frames_found >= frames)
      {
        return bit*0x1000;
      }
    }
    else
    {
      bit = 0;
      frames_found = 0;
    }
  }

  return 0;
}

void pmm_free(uintptr_t ptr, size_t frames)
{
  for (size_t i = 0; i < frames; ++i)
  {
    bitmap_unset_bit(bitmap, ptr/0x1000);
    ptr += 0x1000;
  }
}

void pmm_init(void)
{
  mmap_resp = mmap_req.response;
  init_bitmap();
}
