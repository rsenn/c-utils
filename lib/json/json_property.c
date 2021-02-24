#include "../json_internal.h"

jsonval
json_property(jsonval obj, jsonval name) {
  jsonval ret = json_undefined();

  if(obj.type == JSON_ARRAY || obj.type == JSON_STRING) {
    if(str_equal(json_string_cstr(&name), "length"))
      return json_int(json_length(obj));
  }

  if(obj.type == JSON_OBJECT) {
    stralloc key;
    stralloc_init(&key);
    json_tostring(name, &key);
    stralloc_nul(&key);
    ret = *(jsonval*)MAP_GET(obj.dictv, key.s, key.len);
    stralloc_free(&key);
  } else if(obj.type == JSON_ARRAY) {
    struct slink* p;
    int64 i;

    i = json_toint(name);
    for(p = obj.listv; p; p = p->next) {
      if(i-- == 0) {
        ret = *(jsonval*)slist_data(p);
        break;
      }
    }
  }
  return ret;
}
