/* author: Ian Marco Moffett */

.ifdef __aarch64__

.global global_irq_disable
.global global_irq_enable

.section .text
global_irq_disable:
  mrs x0, daif
  mov x1, #0xC0
  orr x0, x0, x1
  msr daif, x0
  ret

global_irq_enable:
  mrs x0, daif
  mov x1, #~(0xC0)
  and x0, x0, x1
  msr daif, x0
  ret

.endif
