#include "../buffer.h"
#include "../stralloc.h"
#include "../json.h"

static void
json_default_printer(jsonfmt* p, jsonval* v, int depth) {
  p->indent = "  ";
  p->newline = "\n";
  p->spacing = " ";
  p->quote = "\"";
};

void
json_tosa(jsonval val, stralloc* sa, void (*p)(jsonfmt*, jsonval*, int)) {
  buffer b;
  char buf[1024];

  if(p == NULL)
    p = &json_default_printer;

  buffer_init(&b, (int (*)(int, void*, unsigned int, void*)) & stralloc_write, -1, buf, sizeof(buf));
  b.cookie = sa;

  json_print(val, &b, p);
}
