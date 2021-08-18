#include "../xml.h"
#include "../byte.h"
#include "../scan.h"
#include "../buffer.h"

#include <ctype.h>

xmltoken
xml_read_token(xmlscanner* s) {
  buffer* b = s->b;
  char* x;
  size_t i, len;
  xmltoken r;
  xmltokid tok;
  r.x = 0;
  r.len = 0;
again:
  tok = s->tok;
  r.id = tok;
  if(tok == XML_EOF || buffer_EOF(b))
    r.id = XML_EOF;
  x = buffer_PEEK(b);
  len = buffer_LEN(b);
  i = 0;
  switch(tok) {
    case XML_TAG_NAME: {
      size_t j;
      if(byte_equal("<!--", 4, x)) {
        r.id = XML_COMMENT;
        s->tok = XML_DATA;
        j = i = xml_scan_comment(x, len);
      } else {
        j = xml_scan_tag(x, len, &i);
        if(x[j] == '/') {
          s->tok = XML_TAG_CLOSE;
        } else if(x[i] == '>') {
          s->tok = XML_DATA;
        } else {
          s->tok = XML_ATTR_NAME;
        }
      }
      if(r.id == XML_TAG_NAME && x[0] == '<') {
        x++;
        i--;
      }
      b->p += j;
      if(x[i] == '>')
        i++;
      if(*x == '/')
        r.id = XML_TAG_CLOSE;
      break;
    }
    case XML_TAG_CLOSE: {
      size_t j;
      i = byte_chr(x, len, '>');
      if(i < len && x[i] == '>')
        i++;
      s->tok = XML_DATA;
      b->p += i;
      break;
    }
    case XML_ATTR_NAME: {
      size_t j;
      i = scan_noncharsetnskip(x, "=\r\n\t />", len);
      j = i + scan_whitenskip(x + i, len - i);
      if(j < len && byte_chr("/>", 2, x[j]) < 2) {
        s->tok = XML_TAG_CLOSE;
      } else {
        if(j < len && x[j] == '=')
          j++;
        s->tok = XML_ATTR_VALUE;
      }
      b->p += j;
      break;
    }
    case XML_ATTR_VALUE: {
      size_t j;
      char quote[2];
      quote[0] = *x;
      quote[1] = '\0';
      for(i = 1; i < len; i++) {
        i += scan_noncharsetnskip(&x[i], quote, len - i);
        if(i > 0 && x[i - 1] != '\\')
          break;
      }
      j = ++i;
      j += scan_whitenskip(x + j, len - j);
      if(j < len && byte_chr("/>", 2, x[j]) < 2) {
        s->tok = XML_TAG_CLOSE;
      } else {
        s->tok = XML_ATTR_NAME;
      }
      b->p += j;
      break;
    }
    case XML_DATA: {
      size_t i2;
      i2 = byte_chr(x, len, '<');
      /*  i = scan_nonwhitenskip(x, len);
        if(i == 0)
          i = scan_whitenskip(x, len);

        if(i2 < i)*/
      i = i2;
      if(i == i2 && i2 < len) {
        s->tok = XML_TAG_NAME;
        if(i2 == 0)
          goto again;
      }
      b->p += i;
      break;
    }
  }
  if(len == 0)
    r.id = XML_EOF;
  if(r.id == XML_EOF) {
    r.len = 0;
    r.x = 0;
    s->tok = r.id;
    return r;
  }
  r.len = i;
  r.x = x;
  return r;
}
