/* author: Ian Marco Moffett */

#ifndef _SYS_TYPES_H_
#define _SYS_TYPES_H_

#define NULL (void*)0

typedef	signed char		         int8_t;
typedef	unsigned char		       uint8_t;
typedef	short			             int16_t;
typedef	unsigned short		     uint16_t;
typedef	int			               int32_t;
typedef	unsigned int            uint32_t;
#if __SIZEOF_LONG__ == 8
typedef	long                    int64_t;
typedef	unsigned long		       uint64_t;
#elif __SIZEOF_LONG__ == 4
__extension__
typedef	long long		           int64_t;
__extension__
typedef	unsigned long long	   uint64_t;
#else
#error "Unsupported long size"
#endif

#if __SIZEOF_SIZE_T__ == 8
typedef	uint64_t	size_t;
typedef	int64_t	 ssize_t;	/* Byte count or error */
#elif __SIZEOF_SIZE_T__ == 4
typedef	uint32_t	size_t;
typedef	int32_t	 ssize_t;	/* Byte count or error */
#else
#error "Unsupported size_t size"
#endif

typedef size_t uintptr_t;
typedef size_t ptrdiff_t;
typedef size_t off_t;

typedef size_t    id_t;
typedef id_t      pid_t;
typedef id_t      uid_t;
typedef id_t      gid_t;
typedef uint32_t  ino_t;
typedef int32_t   dev_t;
typedef int32_t   mode_t;
typedef int32_t   nlink_t;

#endif
