/* author: Ian Marco Moffett */

#include <amd64/exceptions.h>
#include <amd64/idt.h>
#include <sys/panic.h>

#if defined(__x86_64__)

#define MAKE_ISR(vector, name)               \
  static void exception_##vector(void* sf)   \
  {                                          \
    panic("%s (vector=%x)\n", name, vector); \
  }

#define REGISTER_EXCEPTION(vector) \
  register_exception(vector, (uintptr_t)exception_##vector)

MAKE_ISR(0x0, "Divide error")
MAKE_ISR(0x1, "Debug exception")
MAKE_ISR(0x3, "Breakpoint exception")    /* TODO: Sleep instead of halting */
MAKE_ISR(0x4, "Overflow")
MAKE_ISR(0x5, "BOUND range exceeded")
MAKE_ISR(0x6, "Invalid opcode")
MAKE_ISR(0x7, "Device not available")
MAKE_ISR(0x8, "Double fault")
MAKE_ISR(0xA, "Invalid TSS")
MAKE_ISR(0xB, "Segment not present")
MAKE_ISR(0xC, "Stack segment fault")
MAKE_ISR(0xD, "General protection fault")
MAKE_ISR(0xE, "Page fault")

void exceptions_init(void)
{
  REGISTER_EXCEPTION(0x0);
  REGISTER_EXCEPTION(0x1);
  REGISTER_EXCEPTION(0x3);
  REGISTER_EXCEPTION(0x4);
  REGISTER_EXCEPTION(0x5);
  REGISTER_EXCEPTION(0x6);
  REGISTER_EXCEPTION(0x7);
  REGISTER_EXCEPTION(0x8);
  REGISTER_EXCEPTION(0xA);
  REGISTER_EXCEPTION(0xB);
  REGISTER_EXCEPTION(0xC);
  REGISTER_EXCEPTION(0xD);
  REGISTER_EXCEPTION(0xE);
}

#endif    /* __x86_64__ */
