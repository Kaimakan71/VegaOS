/* author: Ian Marco Moffett */

#ifndef _MM_VMM_H_
#define _MM_VMM_H_

#include <sys/limine.h>
#include <sys/types.h>

extern volatile struct limine_hhdm_request g_hhdm_request;

#define VMM_HIGHER_HALF (g_hhdm_request.response->offset)
#define PAGE_SIZE 0x1000

void vmm_init(void);

#endif
