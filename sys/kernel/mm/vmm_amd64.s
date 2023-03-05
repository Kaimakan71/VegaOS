/* author: Ian Marco Moffett */


.ifdef __x86_64__

.global vmm_switch_vas
.global vmm_get_vas

vmm_switch_vas:
  mov %rdi, %cr3
  retq

vmm_get_vas:
  mov %cr3, %rax
  retq

.endif
