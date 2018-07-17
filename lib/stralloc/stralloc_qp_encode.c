#include <stdint.h>
#include "../stralloc.h"
#include "../byte.h"
#include "../fmt.h"

unsigned int stralloc_qp_encode(stralloc *out, const stralloc *in) {
  static stralloc tmp = { 0, 0, 0 };
  register unsigned int i;
  register unsigned int len;
  char ch;
  unsigned char hex[2];

  len = in->len;
  if(!len) return stralloc_erase(out);
  if(!stralloc_erase(&tmp)) return 0;
  for(i = 0; i < len; i++) {
    ch = in->s[i];
    if(ch == '=') {
      if(!stralloc_catb(&tmp, "=3D", 3)) return 0;
      continue;
    }
    if(char_isprint(ch)) {
      if(!stralloc_append(&tmp, &ch)) return 0;
      continue;
    }
    if(!stralloc_append(&tmp, "=")) return 0;
    fmt_hexb(hex, &ch, 1);
    byte_upper(hex, 2);
    if(!stralloc_catb(&tmp, hex, 2)) return 0;
  }
  return stralloc_copy(out, &tmp);
}
