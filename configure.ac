AC_INIT([VegaOS], [0.0.1], [ian@ian-moffett.com])

AC_CHECK_FILE([cross/bin/x86_64-elf-gcc], [], [AC_MSG_ERROR([Please run tools/cross.sh])])
AC_CHECK_FILE([cross/bin/x86_64-elf-ld], [], [AC_MSG_ERROR([Please run tools/cross.sh])])

CFLAGS="-fexceptions --std=gnu11 -ffreestanding -fno-stack-protector -fno-pic \\
	-Werror=implicit -Werror=implicit-function-declaration \\
	-Werror=implicit-int -Werror=int-conversion \\
	-Werror=incompatible-pointer-types -Werror=int-to-pointer-cast \\
	-Werror=return-type -Wunused -mabi=sysv -mno-80387 -mno-mmx -mno-3dnow \\
	-mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -pedantic \\
	-I sys/include/ -D_KERNEL -Wno-pointer-sign -MMD"

AC_SUBST(CFLAGS, [$CFLAGS])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
