/* author: Ian Marco Moffett */

#ifndef _AARCH64_MMIO_H_
#define _AARCH64_MMIO_H_

#include <sys/types.h>

uintptr_t aarch64_get_mmio_base(void);
void aarch64_mmio_init(void);

#endif
