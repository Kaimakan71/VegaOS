/* author: Ian Marco Moffett */

.ifdef __aarch64__

.global vmm_switch_vas
.global vmm_get_vas

.section .text
vmm_switch_vas:
  msr ttbr0_el1, x0
  tlbi vmalle1is        /* Invalidate TLB */
  dsb ish               /* Ensure TLB invalidation is done */
  isb
  ret

vmm_get_vas:
  mrs x0, ttbr0_el1
  ret

.endif
