#include "../byte.h"
#include "../stralloc.h"
#include "../xml.h"
#include "../str.h"

static int
is_whitespace(const char* x, size_t n) {
  size_t i;

  for(i = 0; i < n; ++i) {
    if(byte_chr(" \t\v\r\n\0", 6, x[i]) == 6)
      return 0;
  }
  return 1;
}

static int
is_name_start(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == ':';
}

/* reads up to and including the terminator end, sa gets what came before it */
static void
read_until(buffer* b, const char* end, size_t n, stralloc* sa) {
  char c;

  stralloc_zero(sa);

  while(buffer_peekc(b, &c) > 0) {
    buffer_skipc(b);
    stralloc_catb(sa, &c, 1);

    if(sa->len >= n && byte_equal(sa->s + sa->len - n, n, end)) {
      sa->len -= n;
      return;
    }
  }
}

/* reads an attribute, its value is empty when there is no equal sign */
static int
read_attribute(buffer* b, stralloc* attr, stralloc* val) {
  const char* s;
  char quote = 0;

  stralloc_zero(attr);
  stralloc_zero(val);

  if(buffer_gettok_sa(b, attr, " \t\r\n\v=/>?", 9) < 0)
    return 0;

  buffer_skipspace(b);

  if((s = buffer_peek(b)) && *s == '=') {
    buffer_skipc(b);
    buffer_skipspace(b);

    if((s = buffer_peek(b)) && (*s == '"' || *s == '\'')) {
      quote = *s;
      buffer_skipc(b);
    }

    if(buffer_gettok_sa(b, val, quote ? &quote : " \t\r\n\v/>", quote ? 1 : 7) < 0)
      return 0;

    if(quote)
      buffer_skipc(b);
  }

  stralloc_nul(attr);
  stralloc_nul(val);
  return 1;
}

/* the text up to the next tag goes to the callback as name and value */
static int
read_text(xmlreader* r, xml_read_callback_fn* fn, stralloc* text) {
  stralloc_zero(text);

  if(buffer_gettok_sa(r->b, text, "<", 1) < 0)
    return 0;

  if(text->len && !is_whitespace(text->s, text->len)) {
    stralloc_nul(text);
    return fn(r, XML_TEXT, text, text, NULL);
  }
  return 1;
}

void
xml_read_callback(xmlreader* r, xml_read_callback_fn* fn) {
  buffer* b = r->b;
  stralloc tag, text, attr, val;
  const char* s;

  stralloc_init(&tag);
  stralloc_init(&text);
  stralloc_init(&attr);
  stralloc_init(&val);

  hmap_init(XML_HMAP_BUCKETS, &r->attrmap);

  while(buffer_skip_until(b, "<", 1) > 0) {
    r->self_closing = r->closing = 0;

    if(!(s = buffer_peek(b)))
      break;

    if(*s == '!') {
      buffer_skipc(b);

      if(!(s = buffer_peek(b)))
        break;

      /* a comment ends at --> and CDATA is text, a doctype ends at the first > */
      if(*s == '-') {
        read_until(b, "-->", 3, &text);
      } else if(*s == '[') {
        read_until(b, "]]>", 3, &text);

        if(text.len > 7 && byte_equal(text.s, 7, "[CDATA[")) {
          byte_copyr(text.s, text.len - 7, text.s + 7);
          text.len -= 7;
          stralloc_nul(&text);

          if(!fn(r, XML_TEXT, &text, &text, NULL))
            break;
        }
      } else {
        read_until(b, ">", 1, &text);
      }
      continue;
    }

    if(*s == '/') {
      r->closing = 1;
      buffer_skipc(b);
    } else if(*s == '?') {
      r->self_closing = 1;
    }

    stralloc_zero(&tag);

    if(buffer_gettok_sa(b, &tag, " \n\t\r\v/>", 7) < 0)
      break;
    stralloc_nul(&tag);

    for(;;) {
      buffer_skipspace(b);

      if(!(s = buffer_peek(b)) || !is_name_start(*s))
        break;

      if(!read_attribute(b, &attr, &val))
        break;

      hmap_set(&r->attrmap, attr.s, attr.len, val.s, val.len + 1);

      if(!fn(r, XML_ATTRIBUTE, &attr, &val, NULL))
        return;
    }

    if((s = buffer_peek(b)) && (*s == '/' || *s == '?')) {
      r->self_closing = 1;
      r->closing = 0;
      buffer_skipc(b);
    }
    buffer_skipspace(b);

    if((s = buffer_peek(b)) && *s == '>')
      buffer_skipc(b);

    fn(r, XML_ELEMENT, &tag, NULL, &r->attrmap);

    if(r->attrmap) {
      hmap_destroy(&r->attrmap);
      r->attrmap = NULL;
    }
    hmap_init(XML_HMAP_BUCKETS, &r->attrmap);

    /* the text after a tag belongs to the element that is open now */
    if((s = buffer_peek(b)) && *s != '<' && !read_text(r, fn, &text))
      break;
  }
}
