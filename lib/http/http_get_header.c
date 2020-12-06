#include "../http.h"
#include "../case.h"
#include "../byte.h"
#include "../scan.h"
#include "../str.h"

const char*
http_get_header(http* h, const char* name) {
  http_response* r;
  size_t pos, len, namelen = str_len(name);
  const char* x;
  if((r = h->response)) {
    pos = byte_chr(r->data.s, r->data.len, '\n');
    if(pos < r->data.len)
      ++pos;
    len = r->data.len - pos;
    x = &r->data.s[pos];
    while(len > 0) {
      size_t pos = byte_chr(x, len, ':');
      if(pos == namelen && !case_diffb(name, pos, x)) {
        x += pos;
        len -= pos;
        if(len > 0) {
          x++;
          len--;
        }
        pos = scan_whitenskip(x, len);
        x += pos;
        len -= pos;
        return x;
      }
      pos = byte_chr(x, len, '\n');
      x += pos;
      len += pos;
      if(len > 0) {
        x++;
        len--;
      }
    }
  }

  return 0;
}