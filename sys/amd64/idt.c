/* author: Ian Marco Moffett */

#include <amd64/idt.h>
#include <sys/bug.h>

#define IDT_TRAP_GATE_FLAGS 0x8F
#define IDT_INT_GATE_FLAGS  0x8E
#define IDT_INT_GATE_USER   0xEE

static struct idt_entry idt[256];
static struct idtr idtr = {
  .limit = sizeof(struct idt_entry) * 256 - 1,
  .offset = (uintptr_t)&idt[0]
};

static void set_desc(uint8_t vector, uintptr_t isr, uint8_t flags)
{
  struct idt_entry *entry = &idt[vector];
  entry->offset_low = (uint16_t)isr;
  entry->offset_mid = (uint16_t)(isr >> 16);
  entry->offset_high = (uint32_t)(isr >> 32);
  entry->reserved = 0;
  entry->zero = 0;
  entry->ist = 0;
  entry->zero1 = 0;
  entry->cs = 0x28;
  entry->dpl = 3;
  entry->p = 1;
  entry->type = flags;
}

void idt_load(void)
{
  __asm("lidt %0" :: "m" (idtr));
}

void register_exception(uint8_t vector, uintptr_t isr)
{
  set_desc(vector, isr, IDT_TRAP_GATE_FLAGS);
}
