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

#ifndef _SYS_PRINTK_H_
#define _SYS_PRINTK_H_

#include <stdarg.h>

#define PRINTK_DEFAULT_FG 0x808080

#define KLOGCOLOR_DEFAULT '0'
#define KLOGCOLOR_RED     '1'
#define KLOGCOLOR_WHITE   '2'
#define KLOGCOLOR_JADE    '3'
#define KLOGCOLOR_YELLOW  '4'
#define KLOGCOLOR_PURPLE  '5'

/*
 *  It is a good idea to use
 *  panic() from sys/panic.h
 */

#define __KERN_PANIC "\0012[\0011 KPANIC\0012 ]\0010 "

/*
 *  Standard kernel message
 *  logging levels.
 */

#define KERN_INFO "\0012[\0013 INFO\0012 ]\0010 "
#define KERN_WARNING "\0012[\0015 WARN\0012 ]\0010 "

/*
 *  kinfo logs with the module name
 *  and nicer formatting.
 *
 *  NOTE: You need to have MODULE("module_name")
 *        at the top of your file.
 *
 *        MODULE can be found in sys/module.h
 */

#define kinfo(fmt, ...)                             \
  printk(KERN_INFO "\0014%s: ", __THIS_MODULE);     \
  printk(fmt, ##__VA_ARGS__);

#define kinfo(fmt, ...)                             \
  printk(KERN_INFO "\0014%s: ", __THIS_MODULE);     \
  printk(fmt, ##__VA_ARGS__);

#define kwarn(fmt, ...)                             \
  printk(KERN_WARNING "\0014%s: ", __THIS_MODULE);  \
  printk(fmt, ##__VA_ARGS__);

void vprintk(const char *fmt, va_list ap);
void printk(const char *fmt, ...);

#endif
