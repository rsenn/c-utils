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

INCLUDEPATH += .

HEADERS = lib/buffer.h lib/byte.h lib/case.h lib/dns.h lib/errmsg.h lib/fmt.h lib/ndelay.h lib/open.h lib/scan.h lib/socket.h lib/str.h lib/stralloc.h lib/tai.h lib/taia.h lib/uint16.h


SOURCES = dnstest.c lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_flush.c lib/buffer/buffer_put.c lib/buffer/buffer_putflush.c lib/buffer/buffer_stubborn.c lib/byte/byte_copy.c lib/byte/byte_diff.c lib/byte/byte_equal.c lib/byte/byte_zero.c lib/case/case_diffb.c lib/dns/dns_dfd.c lib/dns/dns_domain.c lib/dns/dns_ip4.c lib/dns/dns_packet.c lib/dns/dns_random.c lib/dns/dns_rcip.c lib/dns/dns_resolve.c lib/dns/dns_sortip.c lib/dns/dns_transmit.c lib/errmsg/errmsg_iam.c lib/errmsg/errmsg_puts.c lib/errmsg/errmsg_warnsys.c lib/errmsg/errmsg_write.c lib/fmt/fmt_ulong.c lib/io/iopause.c lib/ndelay/ndelay_on.c lib/open/open_read.c lib/scan/scan_fromhex.c lib/scan/scan_ulong.c lib/scan/scan_ulongn.c lib/scan/scan_xlong.c lib/socket/fmt_ip4.c lib/socket/scan_ip4.c lib/socket/scan_ip6.c lib/socket/socket_bind4.c lib/socket/socket_bind6.c lib/socket/socket_connect4.c lib/socket/socket_connect6.c lib/socket/socket_connected.c lib/socket/socket_ip4loopback.c lib/socket/socket_noipv6.c lib/socket/socket_tcp6.c lib/socket/socket_tcp6b.c lib/socket/socket_udp6.c lib/socket/socket_v4mappedprefix.c lib/socket/socket_v6any.c lib/socket/socket_v6loopback.c lib/str/str_len.c lib/stralloc/openreadclose.c lib/stralloc/readclose.c lib/stralloc/stralloc_append.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/tai/tai_pack.c lib/taia/taia_add.c lib/taia/taia_approx.c lib/taia/taia_frac.c lib/taia/taia_less.c lib/taia/taia_now.c lib/taia/taia_pack.c lib/taia/taia_sub.c lib/taia/taia_uint.c lib/uint16/uint16_pack_big.c lib/uint16/uint16_unpack_big.c

DEFINES += HAVE_ERRNO_H=1
