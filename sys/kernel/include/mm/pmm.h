/* author: Ian Marco Moffett */

#ifndef _MM_PMM_H_
#define _MM_PMM_H_

#include <sys/types.h>

void pmm_init(void);
uintptr_t pmm_alloc(size_t frames);
void pmm_free(uintptr_t ptr, size_t frames);


extern volatile struct limine_memmap_request mmap_req;

#endif
