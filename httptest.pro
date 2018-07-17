TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

*msvc* {
  DEFINES += INLINE=__inline
  DEFINES += _CRT_NONSTDC_NO_DEPRECATE=1
  DEFINES += _CRT_SECURE_NO_WARNINGS=1

  QMAKE_CFLAGS_WARN_ON = -W3
}

gcc:   QMAKE_CFLAGS_WARN_ON = -Wno-unused

#mingw | mingw32 | mingw64 | msvc {
win32 | winn64 {
  DEFINES += USE_READDIR=0

} else {
   DEFINES += USE_READDIR=1
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

HEADERS = \
    lib/http.h lib/io.h lib/io_internal.h lib/ndelay.h lib/socket.h \
    lib/buffer.h \
    lib/stralloc.h
SOURCES = httptest.c lib/buffer/buffer_0.c lib/buffer/buffer_0small.c lib/buffer/buffer_1.c lib/buffer/buffer_1small.c lib/buffer/buffer_2.c lib/buffer/buffer_close.c lib/buffer/buffer_default.c lib/buffer/buffer_dummyread.c lib/buffer/buffer_dummyreadmmap.c lib/buffer/buffer_dump.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_free.c lib/buffer/buffer_fromarray.c lib/buffer/buffer_frombuf.c lib/buffer/buffer_fromsa.c lib/buffer/buffer_fromstr.c lib/buffer/buffer_get.c lib/buffer/buffer_getc.c lib/buffer/buffer_getline.c lib/buffer/buffer_getline_sa.c lib/buffer/buffer_getn.c lib/buffer/buffer_getnewline_sa.c lib/buffer/buffer_get_new_token_sa.c lib/buffer/buffer_get_new_token_sa_pred.c lib/buffer/buffer_get_token.c lib/buffer/buffer_get_token_pred.c lib/buffer/buffer_get_token_sa.c lib/buffer/buffer_get_token_sa_pred.c lib/buffer/buffer_get_until.c lib/buffer/buffer_init.c lib/buffer/buffer_init_free.c lib/buffer/buffer_mmapprivate.c lib/buffer/buffer_mmapread.c lib/buffer/buffer_mmapread_fd.c lib/buffer/buffer_munmap.c lib/buffer/buffer_peek.c lib/buffer/buffer_prefetch.c lib/buffer/buffer_put.c lib/buffer/buffer_put8long.c lib/buffer/buffer_putalign.c lib/buffer/buffer_putc.c lib/buffer/buffer_puterror.c lib/buffer/buffer_puterror2.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putlong.c lib/buffer/buffer_putlonglong.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putm_internal_flush.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_putnspace.c lib/buffer/buffer_puts.c lib/buffer/buffer_putsa.c lib/buffer/buffer_putsaflush.c lib/buffer/buffer_putsalign.c lib/buffer/buffer_putsflush.c lib/buffer/buffer_putspace.c lib/buffer/buffer_putuint64.c lib/buffer/buffer_putulong.c lib/buffer/buffer_putulonglong.c lib/buffer/buffer_putxlong.c lib/buffer/buffer_read_fd.c lib/buffer/buffer_seek.c lib/buffer/buffer_skip_until.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/buffer/buffer_tosa.c lib/buffer/buffer_truncfile.c lib/http/http_get.c lib/http/http_init.c lib/http/http_readable.c lib/http/http_sendreq.c lib/http/http_writeable.c lib/io/io_appendfile.c lib/io/io_block.c lib/io/io_canread.c lib/io/io_canwrite.c lib/io/io_check.c lib/io/io_close.c lib/io/io_closeonexec.c lib/io/io_createfile.c lib/io/io_debugstring.c lib/io/io_dontwantread.c lib/io/io_dontwantwrite.c lib/io/io_eagain.c lib/io/io_eagain_read.c lib/io/io_eagain_write.c lib/io/io_fd.c lib/io/io_finishandshutdown.c lib/io/io_getcookie.c lib/io/io_mmapwritefile.c lib/io/io_nonblock.c lib/io/io_pipe.c lib/io/io_readfile.c lib/io/io_readwritefile.c lib/io/io_sendfile.c lib/io/io_setcookie.c lib/io/io_sigpipe.c lib/io/io_socketpair.c lib/io/io_timedout.c lib/io/io_timeout.c lib/io/io_timeouted.c lib/io/io_tryread.c lib/io/io_tryreadtimeout.c lib/io/io_trywrite.c lib/io/io_trywritetimeout.c lib/io/io_wait.c lib/io/io_waitread.c lib/io/io_waituntil.c lib/io/io_waituntil2.c lib/io/io_waitwrite.c lib/io/io_wantread.c lib/io/io_wantwrite.c lib/ndelay/ndelay_off.c lib/ndelay/ndelay_on.c lib/socket/socket_connect4.c lib/socket/socket_connect6.c lib/socket/socket_connected.c lib/socket/socket_ip4loopback.c lib/socket/socket_noipv6.c lib/socket/socket_tcp4.c lib/socket/socket_tcp4b.c lib/socket/socket_tcp6.c lib/socket/socket_tcp6b.c lib/socket/socket_v4mappedprefix.c lib/socket/socket_v6loopback.c lib/stralloc/stralloc_append.c lib/stralloc/stralloc_append_sa.c lib/stralloc/stralloc_cat.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_catc.c lib/stralloc/stralloc_catlong0.c lib/stralloc/stralloc_cats.c lib/stralloc/stralloc_catulong0.c lib/stralloc/stralloc_copy.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_decamelize.c lib/stralloc/stralloc_diffs.c lib/stralloc/stralloc_free.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_insertb.c lib/stralloc/stralloc_move.c lib/stralloc/stralloc_nul.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_remove.c lib/stralloc/stralloc_trunc.c lib/stralloc/stralloc_write.c lib/stralloc/stralloc_zero.c


include(deployment.pri)
qtcAddDeployment()

DISTFILES += \
    mediathek-list.cbp

