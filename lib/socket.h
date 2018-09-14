/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef SOCKET_H
#define SOCKET_H

#include "windoze.h"

#include "uint16.h"
#include "uint32.h"

#include <errno.h>

#if !WINDOWS_NATIVE
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if WINDOWS_NATIVE
#include <windows.h>
typedef int socklen_t;
#if !defined(__ssize_t_defined)
#define __ssize_t_defined 1
typedef SSIZE_T ssize_t;
#endif
#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT WSAEPROTONOSUPPORT
#endif
#ifndef ETIMEDOUT
#define ETIMEDOUT WSAETIMEDOUT
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif
#ifndef EINPROGRESS
#define EINPROGRESS WSAEINPROGRESS
#endif
#ifndef ECONNREFUSED
#define ECONNREFUSED WSAECONNREFUSED
#endif
#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT WSAEPROTONOSUPPORT
#endif
#endif

int socket_tcp4(void);
int socket_tcp4b(void);
int socket_udp4(void);
int socket_udp4b(void);
int socket_tcp6(void);
int socket_tcp6b(void);
int socket_udp6(void);
int socket_udp6b(void);

int socket_sctp4(void);
int socket_sctp4b(void);
int socket_sctp6(void);
int socket_sctp6b(void);

#define socket_tcp() socket_tcp4()
#define socket_udp() socket_udp4()

int socket_connect4(int s, const char* ip, uint16 port);
int socket_connect6(int s, const char* ip, uint16 port, uint32 scope_id);
int socket_connected(int s);
int socket_bind4(int s, const char* ip, uint16 port);
int socket_bind4_reuse(int s, const char* ip, uint16 port);
int socket_bind6(int s, const char* ip, uint16 port, uint32 scope_id);
int socket_bind6_reuse(int s, const char* ip, uint16 port, uint32 scope_id);
int socket_listen(int s, unsigned int backlog);
int socket_accept4(int s, char* ip, uint16* port);
int socket_accept6(int s, char* ip, uint16* port, uint32* scope_id);
ssize_t socket_recv4(int s, char* buf, size_t len, char* ip, uint16* port);
ssize_t socket_recv6(int s, char* buf, size_t len, char* ip, uint16* port, uint32* scope_id);
ssize_t socket_send4(int s, const char* buf, size_t len, const char* ip, uint16 port);
ssize_t socket_send4_flag(int s, const char* buf, size_t len, const char* ip, uint16 port, int flags);
ssize_t socket_send6(int s, const char* buf, size_t len, const char* ip, uint16 port, uint32 scope_id);
ssize_t socket_send6_flag(int s, const char* buf, size_t len, const char* ip, uint16 port, uint32 scope_id, int flags);
int socket_local4(int s, char* ip, uint16* port);
int socket_local6(int s, char* ip, uint16* port, uint32* scope_id);
int socket_remote4(int s, char* ip, uint16* port);
int socket_remote6(int s, char* ip, uint16* port, uint32* scope_id);

/* enable sending udp packets to the broadcast address */
int socket_broadcast(int s);
/* join a multicast group on the given interface */
int socket_mcjoin4(int s, const char* groupip, const char* _interface);
int socket_mcjoin6(int s, const char* groupip, int _interface);
/* leave a multicast group on the given interface */
int socket_mcleave4(int s, const char* groupip);
int socket_mcleave6(int s, const char* groupip);
/* set multicast TTL/hop count for outgoing packets */
int socket_mcttl4(int s, char hops);
int socket_mchopcount6(int s, char hops);
/* enable multicast loopback, pass 1 for enable, 0 for disable */
int socket_mcloop4(int s, char loop);
int socket_mcloop6(int s, char loop);

/* Please note that these are platform specific.  Do not expect them to
 * work.  You might still get an accept() signalled even though there is
 * no data available.  So far, DATAIN is supported on FreeBSD and Linux,
 * and HTTPIN is supported on FreeBSD. */
enum defermode {
  DATAIN, /* only signal accept() if there is data coming in */
  HTTPIN, /* only signal accept() if a HTTP header has come in */
};

void socket_deferaccept(int s, enum defermode mode);

void socket_tryreservein(int s, int size);

const char* socket_getifname(uint32 _interface);
uint32 socket_getifidx(const char* ifname);

extern int noipv6;

/* if HAVE_SOCKET_FASTOPEN is #defined, your version of libowfat
 * has socket_fastopen, socket_fastopen_connect4,
 * socket_fastopen_connect6 and socket_quickack */
#define HAVE_SOCKET_FASTOPEN

/* Turn on server-side TCP fast open support (0 success, -1 error) */
int socket_fastopen(int s);

/* Turn quick ack mode on or off for the socket s. */
int socket_quickack(int s, int value);

/* For client-side TCP fast open, connect and sending the first data is
 * just one step, so we need an API to do it in one step */
ssize_t socket_fastopen_connect4(int s, const char* ip, uint16 port, const char* buf, size_t len);
ssize_t socket_fastopen_connect6(int s, const char* ip, uint16 port, uint32 scope_id, const char* buf, size_t len);

const char* socket_ip6loopback();
const char* socket_ip4loopback();

#ifdef __cplusplus
}
#endif

#endif
