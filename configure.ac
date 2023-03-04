AC_INIT([VegaOS], [0.0.2], [ian@ian-moffett.com])
VEGA_VERSION="0.0.2"

CFLAGS_X86_64="-fexceptions --std=gnu11 -ffreestanding -fno-stack-protector -fno-pic \\
	-Werror=implicit -Werror=implicit-function-declaration \\
	-Werror=implicit-int -Werror=int-conversion \\
	-Werror=incompatible-pointer-types -Werror=int-to-pointer-cast \\
	-Werror=return-type -Wunused -mabi=sysv -mno-80387 -mno-mmx -mno-3dnow \\
	-mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -pedantic \\
	-I sys/kernel/include/ -I sys/kernel/include/posix/ -D_KERNEL -Wno-pointer-sign -MMD"

CFLAGS_AARCH64="-fexceptions --std=gnu11 -ffreestanding -fno-stack-protector -fno-pic \\
	-Werror=implicit -Werror=implicit-function-declaration \\
	-Werror=implicit-int -Werror=int-conversion \\
	-Werror=incompatible-pointer-types -Werror=int-to-pointer-cast \\
	-Werror=return-type -Wunused -pedantic \\
	-I sys/kernel/include/ -I sys/kernel/include/posix/ \\ 
  -D_KERNEL -Wno-pointer-sign -MMD"

AC_ARG_ENABLE([aarch64],
  AS_HELP_STRING([--enable-myfeature], [enable my feature (default is disabled)]),
  [enable_aarch64="$enableval"],
  [enable_aarch64="no"])

if test "x$enable_aarch64" = "xyes"; then
  AC_SUBST(CFLAGS, [$CFLAGS_AARCH64])
  AC_SUBST(ARCH, [aarch64])
else
  AC_SUBST(CFLAGS, [$CFLAGS_X86_64])
  AC_SUBST(ARCH, [x86_64])
fi

AC_SUBST(VEGA_VERSION, [$VEGA_VERSION])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
