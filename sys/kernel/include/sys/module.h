/* author: Ian Marco Moffett */

#ifndef _SYS_MODULE_H_
#define _SYS_MODULE_H_

#include <sys/cdefs.h>

#define MODULE(name) __unused static const char* const __THIS_MODULE = name

#endif
