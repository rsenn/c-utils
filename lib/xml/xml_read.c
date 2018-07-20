#include "../xml.h"
#include "../buffer.h"
#include "../stralloc.h"
#include "../byte.h"
#include <ctype.h>

void
xml_read(xmlreader* r, buffer* b, xml_read_fn* fn) {
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

    while(sa.len > 0 && (isspace(sa.s[sa.len - 1]))) // || sa.s[sa.len - 1] == '>'))
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

    buffer_puts(buffer_1, "<");
    buffer_putsa(buffer_1, &sa);

    ch = *buffer_peek(r->b);


    //    if(ch != '>' && ch != '/') {

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

      buffer_puts(buffer_1, " ");
      buffer_putsa(buffer_1, &attr);
      buffer_puts(buffer_1, "=\"");

      buffer_putsa(buffer_1, &val);
      buffer_puts(buffer_1, "\"");
      //      buffer_putnlflush(buffer_1);

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


    buffer_puts(buffer_1, (!r->closing && r->self_closing) ? "/>" : ">");
    buffer_putnlflush(buffer_1);


    if(!fn(r, XML_NODE_ELEMENT, &sa, NULL, &r->attrmap))
      return;

//    hmap_truncate(&r->attrmap);
    if(r->attrmap) {
      hmap_destroy(&r->attrmap);
      r->attrmap = NULL;
    }

    hmap_init(1024, &r->attrmap);

    if(s[sa.len] != '>') {
      if(buffer_skip_until(r->b, ">", 1) <= 0)
        break;
    }
  }
}
