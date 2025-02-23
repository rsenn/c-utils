/* dns.c - Asynchronous DNS resolver
 *
 * partial rewrite of DNS client code
 * from libowfat by fefe
 * (http://www.fefe.de/libowfat/),
 * originally written and put into
 * public domain July 4 2002
 * (http://online.securityfocus.com/archive/1/280642)
 * by D.J. Bernstein (http://cr.yp.to/).
 *
 * The idea was to provide a simple
 * async resolver API in standard libc
 * style which does not base on crappy
 * resolver functions from ISC
 * libresolv. To keep this piece of code
 * simple, slim and portable I decided
 * to reimplement some of the functions
 * using libc functions/types rather
 * than the ones from fefe.
 *
 * Bug reports and stuff go to
 * <smoli@paranoya.ch>
 *
 * $Id: dns.h,v 1.1.2.1 2002/10/22
 * 19:23:42 smoln Exp $
 */
/* This program is free software; you
 * can redistribute it and/or modify it
 * under the terms of the GNU General
 * Public License as published by the
 * Free Software Foundation; either
 * version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the
 * hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even
 * the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU
 * General Public License for more
 * details.
 *
 * You should have received a copy of
 * the GNU General Public License along
 * with this program; see the file
 * COPYING. If not, write to the Free
 * Software Foundation, 59 Temple Place
 * - Suite 330, Boston, MA 02111-1307,
 * USA.
 */

#ifndef DNS_H_
#define DNS_H_

#include <time.h>
#include <poll.h>
#include <sys/select.h>
#include <netinet/in.h>
#include "lib/byte.h"
#include "lib/dns.h"

#define DNS_C_IN "\0\1"
#define DNS_C_ANY "\0\377"

#define DNS_T_A "\0\1"
#define DNS_T_NS "\0\2"
#define DNS_T_CNAME "\0\5"
#define DNS_T_SOA "\0\6"
#define DNS_T_PTR "\0\14"
#define DNS_T_HINFO "\0\15"
#define DNS_T_MX "\0\17"
#define DNS_T_TXT "\0\20"
#define DNS_T_RP "\0\21"
#define DNS_T_SIG "\0\30"
#define DNS_T_KEY "\0\31"
#define DNS_T_AAAA "\0\34"
#define DNS_T_AXFR "\0\374"
#define DNS_T_ANY "\0\377"

#define DEFAULT_TIMEOUT 20 /* 20 seconds timeout */

typedef enum {
  DNS_ALL, /* do callback on any of the
              events below */
  DNS_TIMEOUT,
  DNS_NOTFOUND,
  DNS_IP4, /* got an ipv4 address */
  DNS_IP6, /* got an ipv6 address */
  DNS_NAME /* got a domain name */
} dns_event_type;

struct dns_resolver;

typedef void(dns_event_handler)(struct dns_resolver* dns, dns_event_type type);

struct dns_resolver {
  struct dns_transmit t;
  /* char* query;
   unsigned int querylen;
   char* packet;
   unsigned int packetlen;
   int s1;
   int tcpstate;
   unsigned int udploop;
   unsigned int curserver;
   time_t deadline;
   unsigned int pos;
   const char* servers;
   struct in6_addr localip;
   unsigned int scope_id;
   char qtype[2];*/
  int ans;

  stralloc result;
  time_t timeout;
  void* userarg;
  dns_event_handler* callback;
};

#define dns_is_idle(dns) (!(dns)->s1)
#define dns_is_busy(dns) ((dns)->s1)

#define DNS_NAME4_DOMAIN 31
//#define DNS_NAME6_DOMAIN (4 * 16 + 10)
#define IP4_FMT 20

/* bzero a resolver struct and set
 * timeout to a sane value */
static __inline__ void
dns_zero(struct dns_resolver* dns) {
  byte_zero(dns, sizeof(struct dns_resolver));
  dns->timeout = DEFAULT_TIMEOUT;
}

/* callbacks not implemented */
#if 0
static __inline__ void dns_set_callback(struct dns_resolver *dns, dns_event_handler *handler) { dns->callback = handler; }
#endif

static __inline__ void
dns_set_userarg(struct dns_resolver* dns, void* userarg) {
  dns->userarg = userarg;
}
static __inline__ void*
dns_get_userarg(struct dns_resolver* dns) {
  return dns->userarg;
}
static __inline__ void
dns_set_timeout(struct dns_resolver* dns, time_t seconds) {
  dns->timeout = seconds;
}

/*
   start an A or a PTR query.
 */
extern int dns_name_lookup(struct dns_resolver* dns, const char* name);
extern int dns_addr_lookup(struct dns_resolver* dns, struct in_addr address);

/*
   returns 1 when the fdset has been
   modified.
 */
extern int dns_pre_poll(struct dns_resolver* dns, struct pollfd* pfds);
extern int dns_pre_select(struct dns_resolver* dns, fd_set* readset, fd_set* writeset);

/*
   returns 0 when lookup is in progress,
   -1 on error/timeout and 1 when an
   answer has been received. pfs must
   point to a free location in your
   pollfd array.
 */
extern int dns_post_poll(struct dns_resolver* dns, const struct pollfd* pfds);
extern int dns_post_select(struct dns_resolver* dns, const fd_set* readset, const fd_set* writeset);

/*
   when the post poll call returned 1
   the answer(s) can be retrieved by
   these functions.
 */
extern int dns_get_addr(struct dns_resolver* dns, struct in_addr* addr);
extern int dns_get_name(struct dns_resolver* dns, char* buf, size_t n);

/*
   free allocated packet buffers, close
   socket etc.
 */
extern void dns_free(struct dns_resolver* dns);

#endif /* DNS_H_ */
