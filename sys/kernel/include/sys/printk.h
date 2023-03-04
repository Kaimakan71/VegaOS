/* author: Ian Marco Moffett */

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
