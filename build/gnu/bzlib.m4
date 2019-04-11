dnl AC_CHECK_BZLIB([action-if-enabled-and-found], [action-if-enabled-and-not-found], [action-if-disabled])

dnl AC_DEFUN([AC_HELP_STRING], [])

AC_DEFUN([AC_CHECK_BZLIB],
[
  # let the user enable/disable the usage of bzlib
  AC_ARG_WITH([bzlib],
    AC_HELP_STRING([--with-bzlib=PATH],[search for bzlib in PATH])
AC_HELP_STRING([--without-bzlib],[disable use of bzlib]),
    
[
  BZLIB_ENABLE="yes"
  BZLIB_DIR="$withval"
],
[
  BZLIB_ENABLE="$withval"
  BZLIB_DIR=""
])
  
  if test "$BZLIB_ENABLE" != no; then
    BZLIB_ENABLE="yes"
  fi
  
  if test -z "$BZLIB_DIR"; then
    BZLIB_DIR='${prefix}'
  fi
  
  # let the user specify a non-standard include path
  AC_ARG_WITH([bzlib-include-dir],
    AC_HELP_STRING([--with-bzlib-include-dir=PATH],[search for bzlib headers in PATH]),
      [BZLIB_INC_DIR="$withval"], [BZLIB_INC_DIR="$BZLIB_DIR/include"])
    
  # let the user specify a non-standard library path
  AC_ARG_WITH([bzlib-lib-dir],
    AC_HELP_STRING([--with-bzlib-lib-dir=PATH],[search for bzlib libraries in PATH]),
      [BZLIB_LIB_DIR="$withval"], [BZLIB_LIB_DIR="yes"])

  # reset bzlib dir if none was specified
  if test "$BZLIB_DIR" = "yes"; then
    BZLIB_ENABLE="yes"
    BZLIB_DIR="$prefix"
  fi
  
  if test "$BZLIB_DIR" = "no"; then
    BZLIB_ENABLE="no"
    BZLIB_DIR=""
  fi
 
  if test "$BZLIB_DIR" = yes; then
    BZLIB_DIR="$ac_default_prefix"
  fi

  # set include dir if specified
  if test -n "$BZLIB_INC_DIR"; then
    BZLIB_CFLAGS="-I$BZLIB_INC_DIR"
  fi
  
  if test ! -d "$BZLIB_LIB_DIR" -o "$BZLIB_LIB_DIR" = yes; then

    for DIR in $BZLIB_DIR/lib64 $BZLIB_DIR/lib/x86_64-linux-gnu $BZLIB_DIR/lib/i386-linux-gnu $BZLIB_DIR/lib; do
      test -d "$DIR" && {
        BZLIB_LIB_DIR="$DIR"
        break
      }
    done
    test -d "$BZLIB_LIB_DIR" || unset BZLIB_LIB_DIR
  fi

  # set lib dir if specified
  if test -n "$BZLIB_LIB_DIR"; then
    BZLIB_LIBS="-L$BZLIB_LIB_DIR"
  fi
  
  if test "$BZLIB_ENABLE" = "yes" || test -z "$BZLIB_ENABLE"; then
    # backup libs and cflags
    save_LIBS="$LIBS"
    save_CFLAGS="$CFLAGS"
  
    # set bzlib libs and cflags
    CFLAGS="$BZLIB_CFLAGS"
    LIBS="$BZLIB_LIBS"
  
    # now check for the library
    AC_CHECK_LIB([bz2], [BZ2_bzDecompress], [BZLIB_OK="yes"], [BZLIB_OK="no"])
  
    # and now for the header
    AC_CHECK_HEADER([bzlib.h], [BZLIB_OK="yes"], [BZLIB_OK="no"])
    
    if test "$BZLIB_OK" = "yes"; then
      :; $1
    else
      :; $2
    fi
  
    # restore the flags
    BZLIB_CFLAGS="$CFLAGS"
    BZLIB_LIBS="$LIBS -lbz2"
  
    CFLAGS="$save_CFLAGS"
    LIBS="$save_LIBS"
    
    AC_SUBST([BZLIB_DIR])
    AC_SUBST([BZLIB_INC_DIR])
    AC_SUBST([BZLIB_LIB_DIR])
    AC_SUBST([BZLIB_CFLAGS])
    AC_SUBST([BZLIB_LIBS])
    AC_SUBST([BZLIB_ENABLE])
  else
    :; $3
  fi])
