#include "../byte.h"
#include "../xml.h"

void
xml_reader(xmlreader* r, buffer* b) {
  byte_zero(r, sizeof(xmlreader));
  r->doc = xml_newnode(XML_DOCUMENT);
  r->ptr = &r->doc->children;
  r->parent = r->doc;
  r->b = b;
}
