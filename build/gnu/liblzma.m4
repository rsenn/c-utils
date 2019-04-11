dnl AC_CHECK_LIBLZMA([action-if-enabled-and-found], [action-if-enabled-and-not-found], [action-if-disabled])

dnl AC_DEFUN([AC_HELP_STRING], [])

AC_DEFUN([AC_CHECK_LIBLZMA],
[
  # let the user enable/disable the usage of liblzma
  AC_ARG_WITH([liblzma],
    AC_HELP_STRING([--with-liblzma=PATH],[search for liblzma in PATH])
AC_HELP_STRING([--without-liblzma],[disable use of liblzma]),
    
[
  LIBLZMA_ENABLE="yes"
  LIBLZMA_DIR="$withval"
],
[
  LIBLZMA_ENABLE="$withval"
  LIBLZMA_DIR=""
])
  
  if test "$LIBLZMA_ENABLE" != no; then
    LIBLZMA_ENABLE="yes"
  fi
  
  if test -z "$LIBLZMA_DIR"; then
    LIBLZMA_DIR='${prefix}'
  fi
  
  # let the user specify a non-standard include path
  AC_ARG_WITH([liblzma-include-dir],
    AC_HELP_STRING([--with-liblzma-include-dir=PATH],[search for liblzma headers in PATH]),
      [LIBLZMA_INC_DIR="$withval"], [LIBLZMA_INC_DIR="$LIBLZMA_DIR/include"])
    
  # let the user specify a non-standard library path
  AC_ARG_WITH([liblzma-lib-dir],
    AC_HELP_STRING([--with-liblzma-lib-dir=PATH],[search for liblzma libraries in PATH]),
      [LIBLZMA_LIB_DIR="$withval"], [LIBLZMA_LIB_DIR="yes"])

  # reset liblzma dir if none was specified
  if test "$LIBLZMA_DIR" = "yes"; then
    LIBLZMA_ENABLE="yes"
    LIBLZMA_DIR="$prefix"
  fi
  
  if test "$LIBLZMA_DIR" = "no"; then
    LIBLZMA_ENABLE="no"
    LIBLZMA_DIR=""
  fi
 
  if test "$LIBLZMA_DIR" = yes; then
    LIBLZMA_DIR="$ac_default_prefix"
  fi

  # set include dir if specified
  if test -n "$LIBLZMA_INC_DIR"; then
    LIBLZMA_CFLAGS="-I$LIBLZMA_INC_DIR"
  fi
  
  if test ! -d "$LIBLZMA_LIB_DIR" -o "$LIBLZMA_LIB_DIR" = yes; then

    for DIR in $LIBLZMA_DIR/lib64 $LIBLZMA_DIR/lib/x86_64-linux-gnu $LIBLZMA_DIR/lib/i386-linux-gnu $LIBLZMA_DIR/lib; do
      test -d "$DIR" && {
        LIBLZMA_LIB_DIR="$DIR"
        break
      }
    done
    test -d "$LIBLZMA_LIB_DIR" || unset LIBLZMA_LIB_DIR
  fi

  # set lib dir if specified
  if test -n "$LIBLZMA_LIB_DIR"; then
    LIBLZMA_LIBS="-L$LIBLZMA_LIB_DIR"
  fi
  
  if test "$LIBLZMA_ENABLE" = "yes" || test -z "$LIBLZMA_ENABLE"; then
    # backup libs and cflags
    save_LIBS="$LIBS"
    save_CFLAGS="$CFLAGS"
  
    # set liblzma libs and cflags
    CFLAGS="$LIBLZMA_CFLAGS"
    LIBS="$LIBLZMA_LIBS"
  
    # now check for the library
    AC_CHECK_LIB([lzma], [lzma_code], [LIBLZMA_OK="yes"], [LIBLZMA_OK="no"])
  
    # and now for the header
    AC_CHECK_HEADER([lzma.h], [LIBLZMA_OK="yes"], [LIBLZMA_OK="no"])
    
    if test "$LIBLZMA_OK" = "yes"; then
      :; $1
    else
      :; $2
    fi
  
    # restore the flags
    LIBLZMA_CFLAGS="$CFLAGS"
    LIBLZMA_LIBS="$LIBS -llzma"
  
    CFLAGS="$save_CFLAGS"
    LIBS="$save_LIBS"
    
    AC_SUBST([LIBLZMA_DIR])
    AC_SUBST([LIBLZMA_INC_DIR])
    AC_SUBST([LIBLZMA_LIB_DIR])
    AC_SUBST([LIBLZMA_CFLAGS])
    AC_SUBST([LIBLZMA_LIBS])
    AC_SUBST([LIBLZMA_ENABLE])
  else
    :; $3
  fi])
