#include "../byte.h"
#include "../buffer.h"
#include "../xml.h"

void
xml_scanner(xmlscanner* s, buffer* b) {
  byte_zero(s, sizeof(xmlscanner));
  s->b = b;
}
