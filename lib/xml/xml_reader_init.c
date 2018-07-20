#include "../xml.h"
#include "../byte.h"

void
xml_reader_init(xmlreader* r, buffer *b) {
  byte_zero(r, sizeof(xmlreader));
  r->ptr = &r->doc;

  textbuf_init(&r->b, b, XML_READ_BUFFER);
}
