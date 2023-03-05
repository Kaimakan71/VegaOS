.ifdef __aarch64__

.global exceptions_init

.section .data
unknown_err_msg: .string "Unknown error"
mem_access_violation_msg: .string "Memory access violation"
svc_msg: .string "SVC not implemented (TODO)"


.section .text
exceptions_init:  
  ldr x0, =vector_tbl
  msr vbar_el1, x0
  ret

isr:
  msr SPSel, #0  /* switch to SP_EL0 */
  mrs x0, esr_el1
  lsr x0, x0, #26
  and x0, x0, #0x3F

  cmp x0, #0
  b.eq unknown_err

  cmp x0, #0x15
  b.eq svc_handler

  cmp x0, #0x25
  b.eq mem_access_violation

  b unknown_err

.macro do_panic msg_label
  ldr x0, =\msg_label
  b panic
.endm

unknown_err:
  do_panic unknown_err_msg

svc_handler:
  do_panic svc_msg

mem_access_violation:
  do_panic mem_access_violation_msg

.align 8
vector_tbl:
  b isr

.endif
