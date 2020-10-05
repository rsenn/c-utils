#include "lib/open.h"
#include "lib/str.h"
#include "lib/byte.h"
#include "lib/dir.h"
#include "lib/ip4.h"
#include "lib/dns.h"

#include <unistd.h>
#include <errno.h>

#include "error.h"
#include "openreadclose.h"
#include "roots.h"

static stralloc data;

static int
roots_find(char* q) {
  int i;
  int j;

  i = 0;
  while(i < data.len) {
    j = dns_domain_length(data.s + i);
    if(dns_domain_equal(data.s + i, q))
      return i + j;
    i += j;
    i += 64;
  }
  return -1;
}

static int
roots_search(char* q) {
  int r;

  for(;;) {
    r = roots_find(q);
    if(r >= 0)
      return r;
    if(!*q)
      return -1; /* user misconfiguration */
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

static int
init2(dir_t* dir) {
  char* d;
  const char* fqdn;
  static char* q;
  static stralloc text;
  char servers[64];
  int serverslen;
  int i;
  int j;

  for(;;) {
    errno = 0;
    d = dir_read(dir);
    if(!d) {
      if(errno)
        return 0;
      return 1;
    }

    if(d[0] != '.') {
      if(openreadclose(d, &text, 32) != 1)
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
          if(serverslen <= 60)
            if(ip4_scan(text.s + j, servers + serverslen))
              serverslen += 4;
          j = i + 1;
        }
      byte_zero(servers + serverslen, 64 - serverslen);

      if(!stralloc_catb(&data, q, dns_domain_length(q)))
        return 0;
      if(!stralloc_catb(&data, servers, 64))
        return 0;
    }
  }
}

static int
init1(void) {
  dir_t dir;
  int r;

  if(chdir("servers") == -1)
    return 0;
  if(dir_open(&dir, "."))
    return 0;
  r = init2(&dir);
  dir_close(&dir);
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
  r = init1();
  if(fchdir(fddir) == -1)
    r = 0;
  close(fddir);
  return r;
}
