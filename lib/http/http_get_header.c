#include "../http.h"
#include "../case.h"
#include "../byte.h"
#include "../scan.h"
#include "../str.h"

char*
http_get_header(char* data, size_t n, const char* name, size_t* result_len) {
  size_t len, p, pos, namelen = str_len(name);
  char* x;

  if((pos = byte_chr(data, n, '\n')) < n)
    ++pos;

  len = n - pos;
  x = &x[pos];

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

      if(result_len)
        *result_len = byte_chrs(x, len, "\r\n", 2);

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

const char*
http_get_header_n(stralloc* data, const char* name, size_t* lenp) {

  return 0;
}
