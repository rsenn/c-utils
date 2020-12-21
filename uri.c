#include "lib/uint16.h"
#include "lib/scan.h"
#include "lib/byte.h"
#include "lib/str.h"
#include "lib/buffer.h"
#include "lib/fmt.h"
#include "lib/stralloc.h"
#include "uri.h"
#include <stdlib.h>
#include <ctype.h>

int
uri_init(uri_t* u) {
  byte_zero(u, sizeof(uri_t));
  return 1;
}

char*
uri_str(const uri_t* u) {
  size_t len = uri_fmt(0, u);
  char* s = malloc(len + 1);
  s[uri_fmt(s, u)] = '\0';
  return s;
}

size_t
uri_scan(uri_t* u, const char* x, size_t len) {
  size_t s, e, e2, e3;
  e = byte_finds(x, len, "://");
  if(e < len) {
    u->proto = str_ndup(x, e);
    s = e + 3;
  } else {
    if(u->proto == 0)
      u->proto = str_dup("http");
    s = 0;
  }
  e = s + byte_chr(&x[s], len - s, '/');
  if(!(s == 0 && e == 0) && !(e == len)) {
    e2 = s + byte_chr(&x[s], e - s, '@');
    if(e2 < e) {
      e3 = s + byte_chr(&x[s], e2 - s, ':');
      if(e3 < e2) {
        u->password = str_ndup(&x[e3 + 1], e2 - (e3 + 1));
      }
      u->username = str_ndup(&x[s], e3 - s);
    }
    e2 = s + byte_chr(&x[s], e - s, ':');
    if(e2 < e) {
      if((e3 = scan_ushort(&x[e2 + 1], &u->port)) == 0)
        u->port = 0;
    } else {
      e3 = s + byte_chr(&x[s], len - s, '/');
    }
    if(e > 0 && e - s > 0)
      u->host = str_ndup(&x[s], e - s);
    if(x[e] == ':') {
      s = e + 1;
      e += s;
    }
    s = e3;
  }
  if(s < len) {
    e2 = s + byte_chrs(&x[s], len - s, "#\r\n\0", 4);
    if(e2 > s) {
      stralloc loc;
      stralloc_init(&loc);
      if(x[s] != '/') {
        if(u->location)
          stralloc_copyb(&loc, u->location, str_rchr(u->location, '/'));
        stralloc_catb(&loc, "/", 1);
        stralloc_catb(&loc, &x[s], e2 - s);
        if(u->location)
          free((void*)u->location);
        u->location = loc.s;
      } else {
        u->location = str_ndup(&x[s], e2 - s);
      }
    }
    s = e2;
  }
  if((e = s + byte_chrs(&x[s], len - s, "#\r\n\0", 4)) < len && x[e] == '#') {
    e2 = e + byte_chrs(&x[e], len - e, "\r\n\0", 3);
    u->anchor = str_ndup(&x[e], e2 - e);
    e = len;
  } else {
    if(u->anchor) {
      free((void*)u->anchor);
      u->anchor = 0;
    }
  }
  if(u->port == 0 && u->proto != 0) {
    if(str_equal(u->proto, "https"))
      u->port = 443;
    else if(str_equal(u->proto, "http"))
      u->port = 80;
  }
  return e;
}

size_t
uri_fmt(char* x, const uri_t* u) {
  size_t r = 0;
  if(u->proto) {
    r += fmt_str(x ? &x[r] : 0, u->proto);
    r += fmt_str(x ? &x[r] : 0, "://");
  }
  if(u->username) {
    r += fmt_str(x ? &x[r] : 0, u->username);
    if(u->password) {
      r += fmt_str(x ? &x[r] : 0, ":");
      r += fmt_str(x ? &x[r] : 0, u->password);
    }
    r += fmt_str(x ? &x[r] : 0, "@");
  }
  r += fmt_str(x ? &x[r] : 0, u->host);
  if(u->port) {
    r += fmt_str(x ? &x[r] : 0, ":");
    r += fmt_ulong(x ? &x[r] : 0, u->port);
  }
  if(u->location)
    r += fmt_str(x ? &x[r] : 0, u->location);
  if(u->anchor)
    r += fmt_str(x ? &x[r] : 0, u->anchor);
  return r;
}

void
uri_free(uri_t* uri) {
  if(uri->proto)
    free((char*)uri->proto);
  if(uri->username)
    free((char*)uri->username);
  if(uri->password)
    free((char*)uri->password);
  if(uri->host)
    free((char*)uri->host);
  if(uri->location)
    free((char*)uri->location);
  if(uri->anchor)
    free((char*)uri->anchor);
  uri->port = 0;
}

void
uri_dump(buffer* b, const uri_t* u) {
  buffer_puts(b, "uri {\n\tproto = ");
  buffer_putstr(b, u->proto ? u->proto : "NULL");
  buffer_puts(b, "\n\tusername = ");
  buffer_putstr(b, u->username);

  buffer_puts(b, "\n\tpassword = ");
  buffer_putstr(b, u->password);

  buffer_puts(b, "\n\thost = ");
  buffer_putstr(b, u->host);

  buffer_puts(b, "\n\tport = ");
  buffer_putulong(b, u->port);

  buffer_puts(b, "\n\tlocation = ");
  buffer_putstr(b, u->location);
  buffer_puts(b, "\n\tanchor = ");
  buffer_putstr(b, u->anchor);
  buffer_puts(b, "\n}");
  buffer_putnlflush(b);
}

void
uri_copy(uri_t* to, const uri_t* from) {

  to->proto = from->proto ? str_dup(from->proto) : 0;
  to->host = from->host ? str_dup(from->host) : 0;
  to->username = from->username ? str_dup(from->username) : 0;
  to->password = from->password ? str_dup(from->password) : 0;
  to->location = from->location ? str_dup(from->location) : 0;
  to->anchor = from->anchor ? str_dup(from->anchor) : 0;
  to->port = from->port;
}

void
uri_clear_anchor(uri_t* uri) {

  if(uri->anchor) {
    free((void*)uri->anchor);
    uri->anchor = 0;
  }
}


size_t
uri_find(const char* x, size_t len) {
  size_t i;
  i = byte_finds(x, len, "://");
  if(i < len) {
    while(i > 0 && (isalnum(x[i - 1]) || x[i - 1] == '+')) i--;
  }
  return i;
}
