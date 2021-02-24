#include "../json_internal.h"

jsonval*
js_property_set(jsonval* obj, jsonval name, jsonval value) {
  jsonval* ret = 0;
  stralloc key;
  stralloc_init(&key);
  json_tostring(name, &key);
  stralloc_nul(&key);
  ret = js_property_set_str(obj, key.s, value);
  stralloc_free(&key);

  if(ret)
    *ret = value;

  return ret;
}
