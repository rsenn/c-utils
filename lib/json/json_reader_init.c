#include "../json.h"
#include "../byte.h"

void
json_reader_init(jsonreader* r, buffer *b) {
  byte_zero(r, sizeof(jsonreader));
  r->doc = json_newnode(JSON_NONE);


  r->b = b;
//  textbuf_init(&r->b, b, JSON_READ_BUFFER);
}
