/* author: Ian Marco Moffett */

#ifndef _AARCH64_CPU_H_
#define _AARCH64_CPU_H_

#include <sys/types.h>
#include <sys/cdefs.h>

#define cpu_read_sysreg(name) __extension__ ({    \
        size_t ret;                               \
        __asm("mrs %0, " #name : "=r" (ret));     \
        ret;                                      \
    })

#define cpu_write_sysreg(name, val) __extension__ ({   \
        __asm("msr " #name ", %0" :: "r" (val));       \
    })

#endif
