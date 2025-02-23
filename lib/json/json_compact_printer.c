#include "../json_internal.h"

void
json_compact_printer(jsonfmt* p, jsonval* v, int depth, int index, char quote) {
  static char q[2] = {'"', '\0'};
  p->indent = "";
  p->newline = " ";
  p->spacing = " ";
  p->separat = ", ";
  p->quote[0] = quote;
  p->quote[1] = quote;
  p->precision = 10;
  p->depth = depth;
  p->index = index;
  p->compliant = 1;
};
