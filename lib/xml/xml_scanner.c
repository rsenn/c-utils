#include "../byte.h"
#include "../buffer.h"
#include "../xml.h"
#include "../alloc.h"

void
xml_scanner(xmlscanner* s, const char* x, size_t len) {
  byte_zero(s, sizeof(xmlscanner));

  s->b = alloc_zero(sizeof(buffer));

  buffer_frombuf(s->b, x, len);
  s->b->deinit = (buffer_deinit*)&alloc_free;
}
