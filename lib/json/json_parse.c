#include "../json_internal.h"
#include "../bool.h"
#include "../byte.h"
#include "../charbuf.h"
#include "../scan.h"
#include "../stralloc.h"

#include <assert.h>
#include <ctype.h>

int
json_parse_getsa(charbuf* b, stralloc* out, bool quoted) {
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

static const char identifier_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789$_";

int
json_parse_null(jsonval* j, charbuf* b) {
  uint8* chars;
  if((chars = charbuf_peekn(b, 5))) {
    if(byte_equal(chars, 4, "null")) {
      if(identifier_chars[str_chr(identifier_chars, chars[4])] == '\0') {
        charbuf_skipn(b, 4);
        j->type = JSON_NULL;
        return 1;
      }
    }
  }
  return 0;
}

int
json_parse(jsonval* j, charbuf* b) {
  int r = 0;
  charbuf_skip_pred(b, &isspace);

  if(!(r = json_object_parse(j, b)))
    if(!(r = json_array_parse(j, b)))
      if(!(r = json_boolean_parse(j, b)))
        if(!(r = json_number_parse(j, b)))
          if(!(r = json_string_parse(j, b)))
            r = json_parse_null(j, b);
  return r;
}
