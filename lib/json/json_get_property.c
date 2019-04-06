#include "../json.h"

jsonval*
json_get_property(jsonval* obj, jsonval name) {
  jsonval* ret = 0;
  if(obj->type == JSON_OBJECT) {
    stralloc key;
    stralloc_init(&key);
    json_tostring(&name, &key);
    ret = hmap_get(&obj->dictv, key.s, key.len);
    stralloc_free(&key);
  }
  return ret;
}

