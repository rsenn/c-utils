#include "../json_internal.h"
#include "../byte.h"
#include "../fmt.h"
#include "../scan.h"

jsonval*
json_read_tree(charbuf* b) {
  jsonreader r;

  json_reader_init(&r, b);
  charbuf_pred_skip(b, charbuf_is_ctype, isspace);
  json_parse(*r.loc, r.b);

  return r.doc;
}
