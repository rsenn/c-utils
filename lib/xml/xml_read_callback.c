#include "../byte.h"
#include "../stralloc.h"
#include "../xml.h"
#include <ctype.h>

static int
is_whitespace(const char* x, size_t n) {
  size_t i;
  for(i = 0; i < n; ++i) {
    if(!isspace(x[i])) return 0;
  }
  return 1;
}

static void
putsa(const char* name, stralloc* tag) {
  size_t i;
  buffer_putm(buffer_1, name, ": ");
  for(i = 0; i < tag->len; ++i) {
    if(tag->s[i] == '\r')
      buffer_puts(buffer_1, "\\r");
    else if(tag->s[i] == '\n')
      buffer_puts(buffer_1, "\\n");
    else if(tag->s[i] == '\t')
      buffer_puts(buffer_1, "\\t");
    else
      buffer_putc(buffer_1, tag->s[i]);
  }
  buffer_putnlflush(buffer_1);
}

void
xml_read_callback(xmlreader* r, xml_read_callback_fn* fn) {
  ssize_t n;
  buffer* b = r->b;
  stralloc tag, attr, val;
  stralloc_init(&tag);
  stralloc_init(&attr);
  stralloc_init(&val);
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
        return;
      continue;
    }

    if((n = buffer_gettok_sa(b, &tag, " \t\r\v/>", 6)) < 0) return;
    stralloc_nul(&tag);
    buffer_skipspace(b);
//    buffer_freshen(b);

    while(isalpha(*(s = buffer_peek(b)))) {
      char ch;
      stralloc_zero(&attr);
      stralloc_zero(&val);
      if((n = buffer_gettok_sa(b, &attr, "=", 1)) < 0) break;
      if(buffer_skipc(b) < 0) return;
      int quoted = 0;
      if(*buffer_peek(b) == '"') {
        if(buffer_skipc(b) < 0) return;
        quoted = 1;
      }
      const char* charset = quoted ? "\"" : "/> \t\r\n\v";
      if((n = buffer_gettok_sa(b, &val, charset, str_len(charset))) < 0)
        break;
      if(quoted && buffer_skipc(b) < 0) return;
      stralloc_nul(&attr);
      stralloc_nul(&val);

  //    buffer_dump(buffer_2, b);

      hmap_set(&r->attrmap, attr.s, attr.len, val.s, val.len + 1);

      if(!fn(r, XML_NODE_ATTRIBUTE, &attr, &val, NULL)) return;

      buffer_skipspace(b);

    }
    buffer_skipspace(b);
    if(str_chr("/?", *buffer_peek(b)) < 2) {
      r->self_closing = 1;
      r->closing = 0;
      buffer_skipc(b);
    }
    buffer_skipspace(b);
    if(*buffer_peek(b) == '>') buffer_skipc(b);
    if(!fn(r, XML_NODE_ELEMENT, &tag, NULL, &r->attrmap)) return;
    if(r->attrmap) {
      hmap_destroy(&r->attrmap);
      r->attrmap = NULL;
    }
    hmap_init(XML_HMAP_BUCKETS, &r->attrmap);
    stralloc_zero(&tag);
    if((n = buffer_gettok_sa(b, &tag, "<", 1)) < 0) return;
    s = buffer_peek(b);
    if(!is_whitespace(tag.s, tag.len)) {
      if(!fn(r, XML_NODE_TEXT, NULL, &tag, NULL)) return;
    }
  }
}
