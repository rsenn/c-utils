#include "../json.h"

int
json_set_property(jsonval* obj, jsonval name, jsonval value) {
  if(obj->type == JSON_OBJECT) {
    stralloc key;
    stralloc_init(&key);
    json_tostring(name, &key);

    return hmap_set(&obj->dictv, key.s, key.len, &value, sizeof(jsonval)) == HMAP_SUCCESS;
  }
  return 0;
}
