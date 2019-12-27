dnl AC_CHECK_ZLIB([action-if-enabled-and-found], [action-if-enabled-and-not-found], [action-if-disabled])

dnl AC_DEFUN([AC_HELP_STRING], [])

AC_DEFUN([AC_CHECK_ZLIB],
[
  # let the user enable/disable the usage of zlib
  AC_ARG_WITH([zlib],
    AC_HELP_STRING([--with-zlib=PATH],[search for zlib in PATH])
AC_HELP_STRING([--without-zlib],[disable use of zlib]),
    
[
  ZLIB_ENANBLE="yes"
  ZLIB_DIR="$withval"
],
[
  ZLIB_ENABLE="$withval"
  ZLIB_DIR=""
])
  
  if test -z "$ZLIB_ENABLE"; then
    ZLIB_ENABLE="yes"
  fi
  
  if test -z "$ZLIB_DIR"; then
    ZLIB_DIR='${prefix}'
  fi

  # let the user specify a non-standard include path
  AC_ARG_WITH([zlib-include-dir],
    AC_HELP_STRING([--with-zlib-include-dir=PATH],[search for zlib headers in PATH]),
      [ZLIB_INC_DIR="$withval"], [ZLIB_INC_DIR="$ZLIB_DIR/include"])
    
  # let the user specify a non-standard library path
  AC_ARG_WITH([zlib-lib-dir],
    AC_HELP_STRING([--with-zlib-lib-dir=PATH],[search for zlib libraries in PATH]),
      [ZLIB_LIB_DIR="$withval"], [ZLIB_LIB_DIR="$ZLIB_DIR/lib"])

  # reset zlib dir if none was specified
  if test "$ZLIB_DIR" = "yes"; then
    ZLIB_ENABLE="yes"
    ZLIB_DIR="$prefix"
  fi
  
  if test "$ZLIB_DIR" = "no"; then
    ZLIB_ENABLE="no"
    ZLIB_DIR=""
  fi
  
  # set include dir if specified
  if test -n "$ZLIB_INC_DIR"; then
    ZLIB_CFLAGS="-I$ZLIB_INC_DIR"
  fi
  
  # set lib dir if specified
  if test -n "$ZLIB_LIB_DIR"; then
    ZLIB_LIBS="-L$ZLIB_LIB_DIR"
  fi
  
  if test "$ZLIB_ENABLE" = "yes" || test -z "$ZLIB_ENABLE"; then
    # backup libs and cflags
    save_LIBS="$LIBS"
    save_CFLAGS="$CFLAGS"
  
    # set zlib libs and cflags
    CFLAGS="$ZLIB_CFLAGS"
    LIBS="$ZLIB_LIBS"
  
    # now check for the library
    AC_CHECK_LIB([z], [inflate], [ZLIB_OK="yes"], [ZLIB_OK="no"])
  
    # and now for the header
    AC_CHECK_HEADER([zlib.h], [ZLIB_OK="yes"], [ZLIB_OK="no"])
    
    if test "$ZLIB_OK" = "yes"; then
      :; $1
    else
      :; $2
    fi
  
    # restore the flags
    ZLIB_CFLAGS="$CFLAGS"
    ZLIB_LIBS="$LIBS -lz"
  
    CFLAGS="$save_CFLAGS"
    LIBS="$save_LIBS"
    
    AC_SUBST([ZLIB_DIR])
    AC_SUBST([ZLIB_INC_DIR])
    AC_SUBST([ZLIB_LIB_DIR])
    AC_SUBST([ZLIB_CFLAGS])
    AC_SUBST([ZLIB_LIBS])
    AC_SUBST([ZLIB_ENABLE])
  else
    :; $3
  fi])
