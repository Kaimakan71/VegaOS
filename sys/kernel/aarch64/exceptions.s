/*
 * Copyright (c) 2023 Ian Marco Moffett and the VegaOS team.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of VegaOS nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

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

.align 11
vector_tbl:
  b isr

.endif
