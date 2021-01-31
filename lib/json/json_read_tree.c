#include "../byte.h"
#include "../fmt.h"
#include "../json.h"
#include "../scan.h"
#include "../map.h"

static int
json_read_value(jsonreader* reader,
                jsondata id,
                stralloc* name,
                stralloc* value,
                MAP_T* attrs) {
  return 1;
}

jsonval*
json_read_tree(charbuf* b) {
  jsonreader r;
  json_reader_init(&r, b);
  *r.loc = json_newnode(JSON_UNDEFINED);
  json_parse(*r.loc, r.b);
  json_read_callback(&r, json_read_value);
  return r.doc;
}
