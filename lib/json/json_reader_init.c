#include "../json_internal.h"
#include "../byte.h"
void
json_reader_init(jsonreader* r, charbuf* b) {
  byte_zero(r, sizeof(jsonreader));
  r->doc = json_newnode(JSON_UNDEFINED);
  r->b = b;
  r->loc = &r->doc;
}
