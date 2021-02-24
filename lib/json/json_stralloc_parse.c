#include "../json_internal.h"
#include "../scan.h"

int
json_stralloc_parse(charbuf* b, stralloc* out, int quoted) {
  unsigned char ch;
  int ret;
  size_t i;
  stralloc sa;
  stralloc_init(&sa);

  for(; (ret = charbuf_peekc(b, &ch)) > 0; charbuf_skip(b)) {
    if(ch == '\\') {
      if((ret = charbuf_nextc(b, &ch)) <= 0)
        break;
    } else if(quoted && ch == '"') {
      charbuf_skip(b);
      ret = 1;
      break;
    } else if(!quoted && ch == ':') {
      ret = sa.len > 0;
      break;
    }
    stralloc_APPEND(&sa, &ch);
  }
  if(quoted || sa.len) {
    for(i = 0; i < sa.len; i++) {
      size_t r;
      unsigned int n;
      if((r = scan_utf8(&sa.s[i], sa.len - i, &n)) > 1) {
        stralloc_cats(out, "\\u");
        stralloc_catxlong0(out, n, n > 0xffffffu ? 8 : n > 0xffffu ? 6 : 4);
        i += r - 1;
      } else {
        ch = n;
        stralloc_APPEND(out, &ch);
      }
    }
  }

  stralloc_free(&sa);
  return ret;
}
