#include "../byte.h"
#include "../buffer.h"
#include "../xml.h"

void
xml_scanner_buf(xmlscanner* s, buffer* b) {
  byte_zero(s, sizeof(xmlscanner));
  s->b = b;
}
