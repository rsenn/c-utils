#include "../json_internal.h"
#include "../scan.h"
#include "../fmt.h"

/* appends the code point as UTF-8 */
static void
put_utf8(stralloc* out, unsigned long u) {
  char tmp[8];
  size_t n = fmt_utf8(tmp, (uint32)u);

  stralloc_catb(out, tmp, n);
}

/*
 * Reads the characters of a string (the quote that starts it is already
 * skipped) and appends the text they stand for: the escapes \" \\ \/ \b \f
 * \n \r \t and \uXXXX are decoded, a pair of \u escapes that makes a
 * surrogate pair gives one code point, a lone surrogate gives U+FFFD. The
 * result is UTF-8, the same bytes the string has in the document.
 * With quoted the string ends at the closing quote, otherwise at a colon.
 */
int
json_parse_stralloc(charbuf* b, stralloc* out, int quoted) {
  unsigned char ch;
  int ret = 0;
  unsigned long high = 0; /* a high surrogate waiting for its low half */

  for(; (ret = charbuf_peekc(b, &ch)) > 0; charbuf_skip(b)) {
    if(ch == '\\') {
      if((ret = charbuf_nextc(b, &ch)) <= 0)
        break;

      switch(ch) {
        case 'n': ch = '\n'; break;
        case 'r': ch = '\r'; break;
        case 't': ch = '\t'; break;
        case 'b': ch = '\b'; break;
        case 'f': ch = '\f'; break;
        case 'u': {
          unsigned long u = 0;
          int k;

          for(k = 0; k < 4; k++) {
            unsigned char h;
            unsigned long d;

            if((ret = charbuf_nextc(b, &h)) <= 0)
              goto done;

            d = h >= '0' && h <= '9' ? h - '0' : h >= 'a' && h <= 'f' ? h - 'a' + 10 : h >= 'A' && h <= 'F' ? h - 'A' + 10 : 16;

            if(d > 15) {
              /* not a hex digit, the escape is damaged */
              u = 0xfffd;
              break;
            }
            u = u << 4 | d;
          }

          if(high && u >= 0xdc00 && u < 0xe000) {
            put_utf8(out, 0x10000 + ((high - 0xd800) << 10) + (u - 0xdc00));
            high = 0;
            continue;
          }

          if(high) {
            put_utf8(out, 0xfffd);
            high = 0;
          }

          if(u >= 0xd800 && u < 0xdc00)
            high = u;
          else if(u >= 0xdc00 && u < 0xe000)
            put_utf8(out, 0xfffd);
          else
            put_utf8(out, u);
          continue;
        }
        default: break;
      }
    } else if(quoted && ch == '"') {
      charbuf_skip(b);
      ret = 1;
      break;
    } else if(!quoted && ch == ':') {
      ret = out->len > 0;
      break;
    }

    if(high) {
      put_utf8(out, 0xfffd);
      high = 0;
    }
    stralloc_APPEND(out, &ch);
  }

done:
  if(high)
    put_utf8(out, 0xfffd);

  return ret;
}
