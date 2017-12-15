AC_PREREQ([2.69])
AC_INIT([sigtime], [0.1])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([strerror])

CFLAGS="$CFLAGS -Wall -O2"
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
