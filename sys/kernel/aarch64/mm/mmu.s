/* author: Ian Marco Moffett */

.ifdef __aarch64__

aarch64_mmu_init:
  /* Extract the PML4 table address */
  mrs x0, ttbr0_el1
  and x0, x0, #0xfffffffffffff000   /* 11:0 -> UNK/SBZP */
  
  ldr x1, =0x1000
  lsr x1, x1, #30
  and x1, x1, #0x1FF
  add x0, x0, x1      /* X0 now contains the PML4 entry */



.endif
