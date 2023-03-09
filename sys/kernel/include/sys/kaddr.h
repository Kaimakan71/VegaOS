/* author: Ian Marco Moffett */

#ifndef _SYS_KADDR_H_
#define _SYS_KADDR_H_

#include <sys/types.h>

/* .text */
extern const uintptr_t __ktext_base;
extern const uintptr_t __ktext_end;

/* .rodata */
extern const uintptr_t __krodata_base;
extern const uintptr_t __krodata_end;


/* .data/.bss */
extern const uintptr_t __kdata_base;
extern const uintptr_t __kdata_end;

#endif
