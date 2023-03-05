/* author: Ian Marco Moffett */

#ifndef _MM_VMM_H_
#define _MM_VMM_H_

#include <sys/limine.h>

extern volatile struct limine_hhdm_request g_hhdm_request;

#define VMM_HIGHER_HALF (g_hhdm_request.response->offset)

#endif
