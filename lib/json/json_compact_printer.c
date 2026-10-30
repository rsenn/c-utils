#include "../json_internal.h"

/* no white space at all, a quote of 0 (used below the top level) means the
 * standard double quote */
void
json_compact_printer(jsonfmt* p, jsonval* v, int depth, int index, char quote) {
  p->indent = "";
  p->newline = "";
  p->spacing = "";
  p->separat = ",";
  p->quote[0] = quote ? quote : '"';
  p->quote[1] = p->quote[0];
  p->precision = 10;
  p->depth = depth;
  p->index = index;
  p->compliant = 1;
}
