#ifndef _SYS_BUG_H_
#define _SYS_BUG_H_

#include <sys/panic.h>

/*
 *  Fails if !condition
 *
 *  Don't use this unless
 *  you NEED to.
 */

#define bug()                                             \
  panic("BUG: %s:%d/%s\n", __FILE__, __LINE__, __func__); \

#endif
