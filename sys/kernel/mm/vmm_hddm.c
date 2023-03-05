/* author: Ian Marco Moffett */

#include <mm/vmm.h>

volatile struct limine_hhdm_request g_hhdm_request = {
  .id = LIMINE_HHDM_REQUEST,
  .revision = 0
};
