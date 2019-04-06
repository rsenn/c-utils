#include "../buffer.h"
#include "../stralloc.h"
#include "../json.h"

static const jsonprinter json_default_printer = {
  "", "", "", ","
};


void
json_tosa(jsonval* val, stralloc* sa, const jsonprinter* p) {
  buffer b;
  char buf[1024];

  if(p == NULL)
    p = &json_default_printer;

  buffer_init(&b, &stralloc_write, -1, buf, sizeof(buf));
  b.cookie = sa;

  json_print(val, &b, p);
}
