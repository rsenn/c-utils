#include "../byte.h"
#include "../stralloc.h"
#include "../buffer.h"
#include "../xml.h"
#include "../alloc.h"

void
xml_scanner_sa(xmlscanner* s, const stralloc* sa) {
  byte_zero(s, sizeof(xmlscanner));

  s->b = alloc_zero(sizeof(buffer));

  buffer_fromsa(s->b, sa);
  s->b->deinit = &alloc_free;
}
