#include "../json_internal.h"

 void
json_default_printer(jsonfmt* p, jsonval* v, int depth, int index, char quote) {
  int pretty = v && get_depth(v) > 1;
  static char q[2] = {'"', '\0'};
  p->indent = pretty ? " " : "";
  p->newline = pretty ? "\n" : "";
  p->spacing = pretty ? " " : "";
  p->separat = pretty ? ", " : ",";
  p->quote[0] = quote;
  p->quote[1] = quote;
  p->precision = 10;
  p->depth = depth;
  p->index = index;
  p->compliant = 1;
};
