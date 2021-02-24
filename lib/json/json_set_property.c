#include "../json_internal.h"

jsonval*
json_set_property(jsonval* obj, jsonval name, jsonval value) {
  jsonval* ret = 0;
  stralloc key;
  stralloc_init(&key);
  json_tostring(name, &key);
  stralloc_nul(&key);
  ret = json_set_property_str(obj, key.s, value);
  stralloc_free(&key);

  if(ret)
    *ret = value;

  return ret;
}
