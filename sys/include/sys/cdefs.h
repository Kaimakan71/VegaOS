/* author: Ian Marco Moffett */

#ifndef _SYS_CDEFS_H_
#define _SYS_CDEFS_H_

#define __asm       __asm__ __volatile__
#define __naked     __attribute__((naked))
#define __packed    __attribute__((packed))
#define __dead      __attribute__((noreturn))

#endif
