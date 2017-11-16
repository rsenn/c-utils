TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig
PKGCONFIG += libxml-2.0

*msvc* {
  DEFINES += INLINE=__inline
  DEFINES += _CRT_NONSTDC_NO_DEPRECATE=1
  DEFINES += _CRT_SECURE_NO_WARNINGS=1

  QMAKE_CFLAGS_WARN_ON = -W3
}

mingw | mingw32 | mingw64 | msvc {
  DEFINES += USE_READDIR=0
}

#win32|win64|msvc|mingw32:CONFIG(release, debug|release):  LIBS += -ladvapi32 -lws2_32
msvc: LIBS += advapi32.lib ws2_32.lib
mingw32|mingw64: LIBS += -ladvapi32 -lws2_32

mingw | mingw32 | mingw64 {
  QMAKE_LFLAGS += -static-libgcc -static-libstdc++
}

!*msvc* {
  DEFINES += INLINE=inline

  #!*mingw*: DEFINES += USE_READDIR=1

  #QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-parameter
}

INCLUDEPATH += . $$PWD/lib

win32: LIBXML2_DIR = e:/Libraries/libxml2-2.9.4/vs2015-x86
win64: LIBXML2_DIR = e:/Libraries/libxml2-2.9.4/vs2015-x64

DEFINES += LIBXML_STATIC=1
INCLUDEPATH += $$LIBXML2_DIR/include $$LIBXML2_DIR/include/libxml
!unix: LIBS += $$LIBXML2_DIR/lib/xml2.lib

#WIN_ICONV_DIR = e:/Libraries/win-iconv-0.0.6/x86_64-w64-mingw32
#INCLUDEPATH += $$WIN_ICONV_DIR/include

HEADERS = \
  lib/byte.h \
  lib/hmap.h \
  lib/stralloc.h \
  lib/str.h \


SOURCES = \
    eagle-init-brd.c \
    lib/byte_copy.c \
    lib/hmap_add.c \
    lib/hmap_destroy.c \
    lib/hmap_free_data.c \
    lib/hmap_init.c \
    lib/hmap_search.c \
    lib/hmap_truncate.c \
    lib/scan_double.c \
    lib/stralloc_catb.c \
    lib/stralloc_cats.c \
    lib/stralloc_copyb.c \
    lib/stralloc_copys.c \
    lib/stralloc_free.c \
    lib/stralloc_init.c \
    lib/stralloc_nul.c \
    lib/stralloc_ready.c \
    lib/stralloc_readyplus.c \
    lib/stralloc_zero.c \
    lib/str_diff.c \
    lib/str_len.c \


include(deployment.pri)
qtcAddDeployment()



