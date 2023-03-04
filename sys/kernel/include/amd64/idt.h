/* author: Ian Marco Moffett */

#ifndef _AMD64_IDT_H_
#define _AMD64_IDT_H_

#include <sys/types.h>
#include <sys/cdefs.h>

struct idt_entry
{
  uint16_t offset_low;
  uint16_t cs;
  uint8_t ist   : 3;
  uint8_t zero  : 5;
  uint8_t type  : 4;
  uint8_t zero1 : 1;
  uint8_t dpl   : 2;
  uint8_t p     : 1;
  uint16_t offset_mid;
  uint32_t offset_high;
  uint32_t reserved;
};

struct __packed idtr
{
  uint16_t limit;
  uintptr_t offset;
};

void idt_load(void);
void register_exception(uint8_t vector, uintptr_t isr);

#endif
