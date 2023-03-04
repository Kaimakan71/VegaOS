/*
 *  author: Ian Marco Moffett.
 *  description: Processor abstraction layer.
 *
 *  Abstracts instructions or processor specific
 *  things into single macros or functions.
 */

#ifndef _SYS_PAL_H_
#define _SYS_PAL_H_

#include <sys/cdefs.h>

#if defined(__x86_64__)

/*
 *  halt() only halts the current processor
 *  and it will continue if an interrupt
 *  fires. To fully halt the CPU ensure
 *  you use global_irq_disable() first.
 */

# define halt()                  __asm("hlt")

/*
 *  Prevents the processor from responding
 *  to external interrupts.
 *
 *  NOTE: This can cause a significant performance
 *        impact, use judiciously.
 */

# define global_irq_disable()    __asm("cli")

/*
 *  Allows the processor to respond
 *  to interrupts.
 */

# define global_irq_enable()     __asm("sti")

#elif defined(__aarch64__)

# define halt()                  __asm("wfi")
# define global_irq_disable()    __asm("msr daifset, #2")
# define global_irq_enable()     __asm("msr daifclr, #2")
#endif    /* __x86_64__  */
#endif    /* _SYS_PAL_H_ */
