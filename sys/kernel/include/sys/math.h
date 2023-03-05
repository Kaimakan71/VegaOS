/* author: Ian Marco Moffett */

#ifndef _SYS_MATH_H_
#define _SYS_MATH_H_

#define ALIGN_DOWN(address, align)  ((address) & ~((align)-1))
#define ALIGN_UP(address, align)    (((address) + (align)-1) & ~((align)-1))

#endif
