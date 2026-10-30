#include "../uri.h"
#include "../scan.h"
#include "../byte.h"
#include "../str.h"
#include "../stralloc.h"
#include "../alloc.h"

static const char* const host_delim = "\r\n:,'\"/#";

static size_t
uri_scan_host(const char* x, size_t len) {
  const char* s = x;

  while(len > 0) {
    if(host_delim[byte_chr(host_delim, str_len(host_delim), *s)])
      break;
    s++;
    len--;
  }
  return (size_t)(s - x);
}

size_t
uri_scan(uri_t* u, const char* x, size_t len) {
  size_t s, e, e2, e3;

  alloc_clear(&u->anchor);
  e = byte_finds(x, len, "://");

  if(e < len) {
    u->proto = str_ndup(x, e);
    s = e + 3;
  } else {
    if(u->proto == 0)
      u->proto = str_dup("http");
    s = 0;
  }

  e = s + byte_chrs(&x[s], len - s, "/", 1);

  if(!(s == 0 && e == 0) /*&& !(e == len)*/) {
    e2 = s + byte_chr(&x[s], e - s, '@');
    u->port = 0;
    alloc_clear(&u->host);
    alloc_clear(&u->username);
    alloc_clear(&u->password);

    if(e2 < e) {
      e3 = s + byte_chr(&x[s], e2 - s, ':');

      if(e3 < e2) {
        u->password = str_ndup(&x[e3 + 1], e2 - (e3 + 1));
      }
      u->username = str_ndup(&x[s], e3 - s);
      s = e2 + 1;
    }

    e2 = s + uri_scan_host(&x[s], len - s);

    if(e2 > s) {
      u->host = str_ndup(&x[s], e2 - s);

      if(x[e2] == ':') {
        if((e3 = scan_ushort(&x[e2 + 1], &u->port)) > 0)
          e2 += 1 + e3;
      }

      s = e2;
    }
  } else {
    s = e;
  }

  if(s == len) {
    alloc_clear(&u->location);
    u->location = str_dup("/");

  } else if(s < len) {
    e2 = s + byte_chrs(&x[s], len - s, "#\r\n\t\"'\0", 7);
    /*if(e2 < len && x[e2] != '#') {
    }*/
    alloc_clear(&u->location);

    if(e2 > s) {
      stralloc loc;
      stralloc_init(&loc);

      if(x[s] != '/') {
        if(u->location)
          stralloc_copyb(&loc, u->location, str_rchr(u->location, '/'));
        stralloc_catb(&loc, "/", 1);
        stralloc_catb(&loc, &x[s], e2 - s);

        u->location = loc.s;
      } else {
        /*if(u->location)
          free((void*)u->location);*/
        u->location = str_ndup(&x[s], e2 - s);
      }
      s = e = e2;
    }
  }

  if(s < len && x[s] == '#') {
    e = s + byte_chrs(&x[s], len - s, "\r\n\t\"'\0", 7);
    u->anchor = str_ndup(&x[s], e - s);
    } else {
    alloc_clear(&u->anchor);
  }

  if(u->port == 0 && u->proto != 0) {
    if(str_equal(u->proto, "https"))
      u->port = 443;
    else if(str_equal(u->proto, "http"))
      u->port = 80;
  }

  return e;
}
