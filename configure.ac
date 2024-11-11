dnl Process this file with autoconf to produce a configure script.
AC_INIT([fl-cow],[0.10])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS(config.h)

AC_CONFIG_MACRO_DIRS([m4])

dnl Checks for programs.
AC_PROG_MAKE_SET
AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CC
AC_PROG_INSTALL
LT_INIT

dnl Checks for libraries.
AC_SEARCH_LIBS(dlopen, dl)

# Autoupdate added the next two lines to ensure that your configure
# script's behavior did not change.  They are probably safe to remove.
AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP


dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_C_VOLATILE
AC_C_CONST
AC_C_BIGENDIAN

dnl Checks for library functions.
AC_CHECK_FUNCS(dladdr open64 memset memcpy getcwd strlen strncmp)


AC_CONFIG_FILES([Makefile fl-cow/Makefile test/Makefile])
AC_OUTPUT

