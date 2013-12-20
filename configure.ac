#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([chimp], [0.1.0], [vilkeliskis.t@gmail.com])
AM_SILENT_RULES([no])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR(config)
AM_INIT_AUTOMAKE(-Wall -Werror tar-ustar dist-zip foreign subdir-objects)

# This lets us use PACKAGE_VERSION in Makefiles
AC_SUBST(PACKAGE_VERSION)

# Take a copy of original flags
CHIMP_ORIG_CFLAGS="${CFLAGS:-none}"
CHIMP_ORIG_CPPFLAGS="${CPPFLAGS:-none}"
CHIMP_ORIG_CXXFLAGS="${CXXFLAGS:-none}"

# Checks for programs.
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11
AC_PROG_CC

CFLAGS="$CFLAGS -Werror -Wall"
CXXFLAGS="$CXXFLAGS -Werror -Wall"

# Checks for libraries.
AC_CHECK_LIB([uv], [uv_tcp_init])
AC_CHECK_LIB([bsd], [strlcpy])
AC_CHECK_LIB([msgpack], [msgpack_version])

# Checks for header files.
AC_CHECK_HEADERS([limits.h stddef.h stdint.h stdlib.h string.h uv.h msgpack.h])

AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([boost/math/complex.hpp], , AC_MSG_ERROR([boost math is required]))
AC_CHECK_HEADERS([armadillo], , AC_MSG_ERROR([libarmadillo is required]))
AC_LANG_POP([C++])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([memset strtoul])

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])
AC_OUTPUT
