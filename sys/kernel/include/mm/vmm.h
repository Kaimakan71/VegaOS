/* author: Ian Marco Moffett */

#ifndef _MM_VMM_H_
#define _MM_VMM_H_

#include <sys/limine.h>
#include <sys/types.h>

extern volatile struct limine_hhdm_request g_hhdm_request;

#define VMM_HIGHER_HALF (g_hhdm_request.response->offset)
#define PAGE_SIZE 0x1000


/* Use these for vmm_map_page() flags argument */
#define VMM_WRITABLE  (1ULL << 1)
#define VMM_NX        (1ULL << 63)


/*
 *  Returns physical address
 *  of top level paging structure.
 */

uintptr_t vmm_get_vas(void);

/*
 *  Switches virtual address space.
 *
 *  phys: Physical address of top
 *        level paging structure.
 */

uintptr_t vmm_switch_vas(uintptr_t phys);

/*
 *  Maps a virtual address
 *  to a physical address.
 */

void vmm_map_page(uintptr_t pagemap_phys, uintptr_t virt, uintptr_t phys,
                  size_t flags);

#endif
