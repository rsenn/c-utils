#include "../byte.h"
#include "../stralloc.h"
#include "../xml.h"
#include <ctype.h>

static int
is_whitespace(const char* x, size_t n) {
  for(size_t i = 0; i < n; ++i) {
    if(!isspace(x[i]))
      return 0;
  }
  return 1;
}

static void
putsa(const char* name, stralloc *sa) {
  buffer_putm(buffer_1, name, ": ");

  for(size_t i = 0; i < sa->len; ++i) {
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
    const char *s;
    stralloc sa;
    stralloc_init(&sa);
    r->self_closing = r->closing = 0;
    
    s = buffer_peek(r->b);

    if(*s == '/') {
      r->closing = 1;
      buffer_skipc(r->b);
    }

    if((n = buffer_gettok_sa(r->b, &sa, " \t\r\v/>", 6)) < 0)
      return;
   

    s = buffer_peek(r->b);

    if(*s == '/') {
      r->self_closing = 1;
      r->closing = 0;
      buffer_skipc(r->b);
//      if((n = buffer_gettok_sa(r->b, &sa, ">", 1)) < 0)
//        return;
    }

    s = buffer_peek(r->b);
    if(*s == '>')
      buffer_skipc(r->b);


//    if(sa.len > 0 && sa.s[sa.len - 1] == '>') {
//    //  *s = '>';
//      sa.len--;
//    } else {
//    //  *s = s[sa.len - 1];
//    }

    if(*s != '>' && *s != '/' && *s != '?') {
      while(isspace(*buffer_peek(r->b))) {
        if(buffer_skipc(r->b) <= 0)
          break;
      }
    }

    s = buffer_peek(r->b);

    while(isalpha(*s)) {
      char ch;

      stralloc attr, val;
      stralloc_init(&attr);
      stralloc_init(&val);

      if((n = buffer_gettok_sa(r->b, &attr, "=", 1)) < 0)
        break;

      if(buffer_skipc(r->b) < 0)
        return;

      int quoted = 0;

      if(buffer_peekc(r->b, &ch) < 0 )
        return;

      if(ch == '"') {
        if(buffer_skipc(r->b) < 0)
          return;
        quoted = 1;
      }
      const char *charset = quoted ? "\"" : "/> \t\r\n\v";

      if((n = buffer_gettok_sa(r->b, &val, charset, str_len(charset))) < 0)
        break;

      if(quoted)
        if(buffer_skipc(r->b) < 0)
          return;

      for(;;) {
        if(buffer_peekc(r->b, &ch) < 0)
          return;
        if(!isspace(ch)) break;
        if(buffer_skipc(r->b) < 0) return;
      }

      s = buffer_peek(r->b);

      hmap_set_stralloc(&r->attrmap, &attr, &val);
      if(!fn(r, XML_NODE_ATTRIBUTE, &attr, &val, NULL))
        return;
    }

    if(sa.s[0] != '/') {
      if(sa.s[0] == '?' || sa.s[sa.len - 1] == '/'  //|| ch == '/'
         )
        r->self_closing = 1;

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

    stralloc_zero(&sa);

    if((n = buffer_gettok_sa(r->b, &sa, "<", 1)) < 0)
      return;

    if(sa.len > 0 && sa.s[sa.len - 1] == '<')
      sa.len--;

    if(!is_whitespace(sa.s, sa.len)) {
      if(!fn(r, XML_NODE_TEXT, NULL, &sa, NULL))
        return;
      //    putsa("text", &sa);
    }
  }
}
