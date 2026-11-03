#define _BSD_SOURCE 1
#define _POSIX_SOURCE 1
#include "../../lib/stralloc.h"
#include "../../lib/open.h"
#include "../../lib/str.h"
#include "../../lib/byte.h"
#include "../../lib/dirlist.h"
#include "../../lib/ip4.h"
#include "../../lib/ip6.h"
#include "../../lib/dns.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif
#include <errno.h>
#include "../../lib/bool.h"

#include "roots.h"

static stralloc data;

int
roots_find(char* q) {
  size_t i, j;

  i = 0;

  while(i < data.len) {
    j = dns_domain_length(&data.s[i]);

    if(dns_domain_equal(&data.s[i], q))
      return i + j;
    i += j;
    i += 64;
  }
  return -1;
}

int
roots_search(char* q) {
  int r;

  for(;;) {
    r = roots_find(q);

    if(r >= 0)
      return r;

    if(!*q)
      return -1; /* user
                    misconfiguration */
    q += *q;
    q += 1;
  }
}

int
roots(char servers[64], char* q) {
  int r;
  r = roots_find(q);

  if(r == -1)
    return 0;
  byte_copy(servers, 64, data.s + r);
  return 1;
}

int
roots_same(char* q, char* q2) {
  return roots_search(q) == roots_search(q2);
}

int
roots_init2(struct dirlist* dl, bool ip6) {
  char *d, *rec;
  const char* fqdn;
  static char* q;
  static stralloc text;
  int iplen = ip6 ? 16 : 4;

  char servers[64];
  size_t serverslen;
  size_t i, j;

  dirlist_foreach(rec, *dl) {
    d = dirlist_name(rec);

    {
      if(openreadclose(d, &text, 1024) != 1)
        return 0;

      if(!stralloc_append(&text, "\n"))
        return 0;

      fqdn = d;

      if(str_equal(fqdn, "@"))
        fqdn = ".";

      if(!dns_domain_fromdot(&q, fqdn, str_len(fqdn)))
        return 0;

      serverslen = 0;
      j = 0;

      for(i = 0; i < text.len; ++i)

        if(text.s[i] == '\n') {
          if(serverslen <= sizeof(servers) - iplen)

            if((ip6 ? scan_ip6 : scan_ip4)(text.s + j, servers + serverslen))
              serverslen += iplen;
          j = i + 1;
        }
      byte_zero(servers + serverslen, sizeof(servers) - serverslen);

      if(!stralloc_catb(&data, q, dns_domain_length(q)))
        return 0;

      if(!stralloc_catb(&data, servers, sizeof(servers)))
        return 0;
    }
  }

  return 1;
}

int
roots_init1(void) {
  arena ar;
  struct dirlist dl;
  int r;

  if(chdir("servers") == -1)
    return 0;

  arena_init(&ar, &arena_heap, 0);
  dl = dirlist_read(&ar, ".", DIRLIST_NOHIDDEN);

  if(!dl.s) {
    arena_free(&ar);
    return 0;
  }

  r = roots_init2(&dl, false);
  arena_free(&ar);
  return r;
}

int
roots_init(void) {
  int fddir;
  int r;

  if(!stralloc_copys(&data, ""))
    return 0;

  fddir = open_read(".");

  if(fddir == -1)
    return 0;
  r = roots_init1();
#if !WINDOWS_NATIVE
  if(fchdir(fddir) == -1)
    r = 0;
#endif
  close(fddir);
  return r;
}
