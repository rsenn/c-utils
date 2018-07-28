#include "../byte.h"
#include "../fmt.h"
#include "../scan.h"
#include "../json.h"


static int
json_read_value(jsonreader* reader, jsondata id, stralloc* name, stralloc* value, HMAP_DB** attrs) {


  return 1;
}

jsonval*
json_read_tree(buffer* b) {
  jsonreader r;
  json_reader_init(&r, b);
  json_read_callback(&r, json_read_value);
  return r.doc;
}
