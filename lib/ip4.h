/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef IP4_H
#define IP4_H

#include <stdlib.h>
#include <sys/types.h>
#include "uint8.h"
#include "uint16.h"
#include "uint32.h"

#ifdef __cplusplus
extern "C" {
#endif

/** IPv4 address */
typedef union {
  /** 32-bit IPv4 address, represented as 4 8-bit \c unsigned \c chars. */
  uint8 addr[4];
  uint32 iaddr;
} ipv4addr;
/** IPv4 port number. */
typedef uint16 ipv4port;

static const ipv4addr IPV4ADDR_ANY = {0, 0, 0, 0};
static const ipv4addr IPV4ADDR_BROADCAST = {255, 255, 255, 255};
static const ipv4addr IPV4ADDR_LOOPBACK = {127, 0, 0, 1};

size_t scan_ip4(const char* src, char* ip);
size_t fmt_ip4(char* dest, const char* ip);

/* for djb backwards compatibility */
#define ip4_scan scan_ip4
#define ip4_fmt fmt_ip4

#define IP4_FMT 20
#define FMT_IP4 20

extern const char ip4loopback[4]; /* = {127,0,0,1};*/

#ifdef __cplusplus
}
#endif

#endif
