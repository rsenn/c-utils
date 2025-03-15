#include "../http.h"
#include "../case.h"
#include "../byte.h"
#include "../scan.h"
#include "../str.h"

const char*
http_get_header(stralloc* data, const char* name) {
  size_t len, p, pos, namelen = str_len(name);
  const char* x;

  if((pos = byte_chr(data->s, data->len, '\n')) < data->len)
    ++pos;

  len = data->len - pos;
  x = &data->s[pos];

  while(len > 0) {
    if((p = byte_chr(x, len, ':')) == namelen && !case_diffb(name, p, x)) {
      x += p;
      len -= p;

      if(len > 0) {
        x++;
        len--;
      }

      p = scan_whitenskip(x, len);
      x += p;
      len -= p;
      return x;
    }

    p = byte_chr(x, len, '\n');
    x += p;
    len += p;

    if(len > 0) {
      x++;
      len--;
    }
  }

  return 0;
}
