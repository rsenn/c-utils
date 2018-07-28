#include "../byte.h"
#include "../fmt.h"
#include "../json.h"
#include "../scan.h"

static int
json_read_value(jsonreader* reader,
                jsondata id,
                stralloc* name,
                stralloc* value,
                HMAP_DB** attrs) {

  return 1;
}

jsonval*
json_read_tree(buffer* b) {
  jsonreader r;
  json_reader_init(&r, b);
  *r.loc = json_newnode(JSON_NONE);
  json_parse(*r.loc, &r.b);
  json_read_callback(&r, json_read_value);
  return r.doc;
}
