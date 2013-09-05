### autoconf configure.ac && autoreconf -if

AC_PREREQ([2.69])
AC_INIT([IPECAC], [0.1])
AC_CONFIG_SRCDIR([include/bitwise.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE


### Checks for programs.
AC_PROG_CC


### Checks for libraries.
AC_CHECK_LIB([m], [sqrt])


# libcheck
AC_ARG_WITH([check], 
  [AS_HELP_STRING(--without-check, Disable libcheck support)], 
  [], 
  [with_check=yes]
)

AS_IF([test "$with_check" != no],
  [AC_CHECK_HEADERS([check.h], 
  AC_CHECK_LIB([check], [suite_create], [], 
  [echo "configure: ERROR: libcheck could not be located."
  exit -1]))
])

AM_CONDITIONAL([WITH_CHECK], [test "$with_check" != no])


# libcobra
AC_ARG_WITH([bench], 
  [AS_HELP_STRING(--without-bench, Disable libcobra support)], 
  [], 
  [with_check=yes]
)

AS_IF([test "$with_bench" != no],
  [AC_CHECK_HEADERS([cobra.h],
  AC_CHECK_LIB([cobra], [bench_run], [], 
  [echo "configure: ERROR: libcobra could not be located."
  exit -1]))
])

AM_CONDITIONAL([WITH_BENCH], [test "$with_bench" != no])


### Checks for additional header files.
AC_CHECK_HEADERS([limits.h stdint.h stdlib.h])


### Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T


### Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC


### Exit
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
