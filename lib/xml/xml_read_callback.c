#include "../xml.h"
#include "../stralloc.h"
#include "../byte.h"
#include <ctype.h>

void
putsa(stralloc* sa) {
  buffer_puts(buffer_1, "sa: ");

  for(int i = 0; i < sa->len; ++i) {
    if(sa->s[i] == '\r')
      buffer_puts(buffer_1, "\\r");
    else if(sa->s[i] == '\n')
      buffer_puts(buffer_1, "\\n");
    else if(sa->s[i] == '\t')
      buffer_puts(buffer_1, "\\t");
    else
      buffer_putc(buffer_1, sa->s[i]);
  }
  buffer_putnlflush(buffer_1);
}


void
xml_read_callback(xmlreader* r, buffer* b, xml_read_fn* fn) {
  ssize_t n;
  byte_zero(r, sizeof(xmlreader));
  r->b = b;
  r->ptr = &r->doc;
  hmap_init(1024, &r->attrmap);

  while((n = buffer_skip_until(r->b, "<", 1)) > 0) {
    char ch;
    const char* s;
    stralloc sa;
    stralloc_init(&sa);
    r->self_closing = r->closing = 0;
    s = buffer_peek(r->b);
    if((n = buffer_get_token_sa(r->b, &sa, " \t\r\v/>", 6)) < 0)
      return;
    if(sa.s[0] == '/') {
      r->closing = 1;
      if((n = buffer_get_token_sa(r->b, &sa, ">", 1)) < 0)
        return;
    } else {
      r->closing = 0;
    }
      sa.len--;
    if(sa.len > 0 && sa.s[sa.len - 1] == '>') {
      ch = '>';
      sa.len--;
    } else {
      ch = s[sa.len - 1];
    }
    if(ch != '>') {
      while(isspace(*buffer_peek(r->b))) {
        if(buffer_getc(r->b, &ch) <= 0)
          break;
      }
    }
    ch = *buffer_peek(r->b);
    while(isalpha(ch)) {
      stralloc attr, val;
      stralloc_init(&attr);
      stralloc_init(&val);
      if((n = buffer_get_token_sa(r->b, &attr, "=", 1)) < 0)
        break;
      if(attr.len > 0 && attr.s[attr.len - 1] == '=')
        attr.len--;
      int quoted = 0;
      ch = *buffer_peek(r->b);
      if(ch == '"') {
        char dummy;
        if(buffer_getc(r->b, &dummy) < 0)
          break;
        quoted = 1;
      }
      const char* charset = quoted ? "\"" : "/> \t\r\n\v";
      if((n = buffer_get_token_sa(r->b, &val, charset, str_len(charset))) < 0)
        break;
      if(val.len > 0) {
        val.len--;
      }
      while(isspace((ch = *buffer_peek(r->b)))) {
        if(buffer_getc(r->b, &ch) <= 0)
          break;
      }
      hmap_set_stralloc(&r->attrmap, &attr, &val);
      if(!fn(r, XML_NODE_ATTRIBUTE, &attr, &val, NULL))
        return;
    }
    if(sa.s[0] != '/') {
      if(sa.s[0] == '?' || sa.s[sa.len - 1] == '/' || ch == '/') {
        r->self_closing = 1;
      }
      if(r->self_closing)
        r->closing = 0;
    }
    if(!fn(r, XML_NODE_ELEMENT, &sa, NULL, &r->attrmap))
      return;
    if(r->attrmap) {
      hmap_destroy(&r->attrmap);
      r->attrmap = NULL;
    }
    hmap_init(1024, &r->attrmap);
    if(s[sa.len] != '>') {
      if(buffer_skip_until(r->b, ">", 1) <= 0)
        break;
    }

    putsa(&sa);

    stralloc_zero(&sa);
    if((n = buffer_gettok_sa(r->b, &sa, "<", 1)) < 0)
      return;

    if(sa.len > 0 && sa.s[sa.len - 1] == '<')
      sa.len--;

    putsa(&sa);


  }
}
