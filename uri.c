#include "lib/uint16.h"
#include "lib/scan.h"
#include "lib/byte.h"
#include "lib/str.h"
#include "lib/buffer.h"
#include "lib/fmt.h"
#include "uri.h"
#include <stdlib.h>

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
uri_scan(const char* x, uri_t* u) {
  size_t s, e, e2, e3;

  e = str_find(x, "://");
  if(x[e]) {
    u->proto = str_ndup(x, e);
    s = e + 3;
  } else {
    u->proto = str_dup("http");
    s = 0;
  }
  e = s + str_chr(&x[s], '/');
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
    if(scan_ushort(&x[e2 + 1], &u->port) == 0)
      u->port = 0;
  }

  u->host = str_ndup(&x[s], e - s);
  if(x[e] == ':') {
    s = e + 1;

    e += s;
  }
  if(x[e] == '/') {
    s = e;
    u->location = str_dup(&x[s]);
    e += str_len(&x[s]);
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
  buffer_puts(b, "\n}");
  buffer_putnlflush(b);
}