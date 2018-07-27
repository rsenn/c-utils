#include "../xml.h"
#include "../byte.h"

void
xml_reader_init(xmlreader* r, buffer *b) {
  byte_zero(r, sizeof(xmlreader));
  r->doc = xml_newnode(XML_DOCUMENT);

  r->ptr = &r->doc->children;

  r->b = b;
//  textbuf_init(&r->b, b, XML_READ_BUFFER);
}
