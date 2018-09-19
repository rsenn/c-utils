AC_INIT([dirlist],[20180919])

AC_CONFIG_SRCDIR([list-r.c])
AC_CONFIG_AUX_DIR([config])

dnl canonicalize the host and os so we can do some tricky things before
dnl parsing options
AC_CANONICAL_HOST
AC_CANONICAL_BUILD

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_INSTALL
AC_LANG_C
AC_EXEEXT

AC_HEADER_STDC

AC_CHECK_HEADERS([alloca.h errno.h inttypes.h linux/limits.h stdbool.h stdint.h sys/fcntl.h])

AC_HEADER_ASSERT
AC_HEADER_DIRENT
AC_HEADER_STAT
AC_HEADER_TIME


AC_FUNC_ALLOCA
AC_FUNC_FNMATCH
AC_FUNC_LSTAT
dnl AC_FUNC_MMAP
dnl AC_FUNC_STRERROR_R

AC_INLINE
AC_C_INLINE

AC_OFF_T
AC_TYPE_OFF_T

AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UNSIGNED_LONG_LONG_INT

AC_LONG_64_BITS

AC_TYPE_INT8_T
AC_TYPE_UINT8_T
AC_TYPE_INT16_T
AC_TYPE_UINT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT64_T
AC_TYPE_INTMAX_T
AC_TYPE_UINTMAX_T
AC_TYPE_INTPTR_T
AC_TYPE_UINTPTR_T

AC_CHECK_FUNC([posix_memalign],[AC_DEFINE_UNQUOTED([HAVE_POSIX_MEMALIGN],[1],[Define this if you have the posix_memalign() function])])
AC_CHECK_FUNC([aligned_alloc],[AC_DEFINE_UNQUOTED([HAVE_ALIGNED_ALLOC],[1],[Define this if you have the aligned_alloc() function])])
AC_CHECK_FUNC([_aligned_malloc],[AC_DEFINE_UNQUOTED([HAVE__ALIGNED_MALLOC],[1],[Define this if you have the _aligned_malloc() function])])
AC_CHECK_FUNC([alloca],[AC_DEFINE_UNQUOTED([HAVE_ALLOCA],[1],[Define this if you have the alloca() function])])
AC_CHECK_FUNC([fnmatch],[AC_DEFINE_UNQUOTED([HAVE_FNMATCH],[1],[Define this if you have the fnmatch() function])])
AC_CHECK_FUNC([inet_pton],[AC_DEFINE_UNQUOTED([HAVE_INET_PTON],[1],[Define this if you have the inet_pton() function])])
AC_CHECK_FUNC([llseek],[AC_DEFINE_UNQUOTED([HAVE_LLSEEK],[1],[Define this if you have the llseek() function])])
AC_CHECK_FUNC([lseek],[AC_DEFINE_UNQUOTED([HAVE_LSEEK],[1],[Define this if you have the lseek() function])])
AC_CHECK_FUNC([lseek64],[AC_DEFINE_UNQUOTED([HAVE_LSEEK64],[1],[Define this if you have the lseek64() function])])
AC_CHECK_FUNC([lstat],[AC_DEFINE_UNQUOTED([HAVE_LSTAT],[1],[Define this if you have the lstat() function])])
AC_CHECK_FUNC([pipe2],[AC_DEFINE_UNQUOTED([HAVE_PIPE2],[1],[Define this if you have the pipe2() function])])
AC_CHECK_FUNC([round],[AC_DEFINE_UNQUOTED([HAVE_ROUND],[1],[Define this if you have the round() function])])
AC_CHECK_FUNC([wordexp],[AC_DEFINE_UNQUOTED([HAVE_WORDEXP],[1],[Define this if you have the wordexp() function])])
AC_CHECK_FUNC([WSAStartup],[AC_DEFINE_UNQUOTED([HAVE_WSASTARTUP],[1],[Define this if you have the WSAStartup() function])])

AC_CHECK_ZLIB
AC_CHECK_BZLIB
AC_CHECK_LZMADEC

AC_CONFIG_HEADERS([config.h])
AC_OUTPUT([Makefile])

