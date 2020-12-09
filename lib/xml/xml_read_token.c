#include "../xml.h"
#include "../byte.h"
#include "../scan.h"

#include <ctype.h>

static int
is_whitespace(const char* x, size_t n) {
  size_t i;

  for(i = 0; i < n; ++i) {
    if(byte_chr(" \t\v\r\n\0", 5, x[i]) == 5)
      return 0;
  }
  return 1;
}

static void
putsa(const char* name, stralloc* tag) {
  size_t i;
  buffer_putm_internal(buffer_1, name, ": ", NULL);

  for(i = 0; i < tag->len; ++i) {
    if(tag->s[i] == '\r')
      buffer_puts(buffer_1, "\\r");
    else if(tag->s[i] == '\n')
      buffer_puts(buffer_1, "\\n");
    else if(tag->s[i] == '\t')
      buffer_puts(buffer_1, "\\t");
    else
      buffer_PUTC(buffer_1, tag->s[i]);
  }
  buffer_putnlflush(buffer_1);
}

char*
xml_read_token(xmlscanner* s, size_t* szptr) {
  buffer* b = s->b;
  char* x;
  size_t i, len;

again:

  if(buffer_skipspace(b) < 0)
    return 0;

  if((x = buffer_peek(b)) == 0)
    return 0;

  len = buffer_LEN(b);

  switch(s->tok) {
    case XML_TAG_NAME: {
      break;
    }
    case XML_TAG_CLOSE: {
      break;
    }
    case XML_ATTR_NAME: {
      break;
    }
    case XML_ATTR_VALUE: {
      break;
    }
    case XML_DATA:
    default: {
      i = byte_chr(x, len, '<');
      if(i == 0 && i < len) {
        if(buffer_skipc(b) <= 0)
          return 0;
        s->tok = XML_TAG_NAME;
        goto again;
      }

      break;
    }

      *szptr = i;
      return x;
  }

  /*
    hmap_init(XML_HMAP_BUCKETS, &r->attrmap);

    while((n = buffer_skip_until(b, "<", 1)) > 0) {
      const char* s;
      stralloc_zero(&tag);
      r->self_closing = r->closing = 0;
      s = buffer_peek(b);

      if(*s == '/') {
        r->closing = 1;
        buffer_skipc(b);
      } else if(*s == '?') {
        r->self_closing = 1;
      } else if(*s == '!') {
        if(buffer_skip_until(b, ">", 1) <= 0)
          break;
        continue;
      }
      if((n = buffer_gettok_sa(b, &tag, " \n\t\r\v/>", 7)) < 0)
        break;
      stralloc_nul(&tag);

      buffer_skipspace(b);

      while((s = buffer_peek(b)) && isalpha(*s)) {
        char ch;
        int quoted = 0;
        const char* charset;
        stralloc_zero(&attr);
        stralloc_zero(&val);
        if((n = buffer_gettok_sa(b, &attr, "=", 1)) < 0)
          break;
        if(buffer_skipc(b) < 0)
          break;

        if(*buffer_peek(b) == '"') {
          if(buffer_skipc(b) < 0)
            break;
          quoted = 1;
        }
        charset = quoted ? "\"" : "/> \t\r\n\v";
        if((n = buffer_gettok_sa(b, &val, charset, str_len(charset))) < 0)
          break;
        if(quoted && buffer_skipc(b) < 0)
          break;
        stralloc_nul(&attr);
        stralloc_nul(&val);
        hmap_set(&r->attrmap, attr.s, attr.len, val.s, val.len + 1);
        if(!fn(r, XML_ATTRIBUTE, &attr, &val, NULL))
          break;
        buffer_skipspace(b);
      }
      buffer_skipspace(b);

      if((s = buffer_peek(b)) && str_chr("/?", *s) < 2) {
        r->self_closing = 1;
        r->closing = 0;
        buffer_skipc(b);
      }
      buffer_skipspace(b);
      if((s = buffer_peek(b)) && *s == '>')
        buffer_skipc(b);

      fn(r, XML_ELEMENT, &tag, NULL, &r->attrmap);
      buffer_skipspace(b);
      if(!(s = buffer_peek(b)))
        break;

      if(*s != '<' && !(r->closing || r->self_closing)) {
        stralloc_zero(&tag);
        buffer_gettok_sa(b, &tag, "<", 1);
        if(tag.len && !is_whitespace(tag.s, tag.len)) {
          stralloc_nul(&tag);
          fn(r, XML_TEXT, &tag, NULL, NULL);
        }
      }
      if(r->attrmap) {
        hmap_destroy(&r->attrmap);
        r->attrmap = NULL;
      }
      hmap_init(XML_HMAP_BUCKETS, &r->attrmap);

      if(b->n - b->p <= 1)
        break;

      s = buffer_peek(b);

      if(!is_whitespace(tag.s, tag.len)) {
        if(!fn(r, XML_TEXT, NULL, &tag, NULL))
          break;
      }
    }*/
}
