#include "../json.h"

jsonval*
json_set_property(jsonval* obj, jsonval name, jsonval value) {
  jsonval* ret = 0;
  if(obj->type == JSON_OBJECT) {
    stralloc key;
    stralloc_init(&key);
    json_tostring(name, &key);
    stralloc_nul(&key);

    /*    buffer_putm_internal(buffer_2, "key: ", key.s, "\n", NULL);
        buffer_flush(buffer_2);*/

    if(MAP_ISNULL(obj->dictv))
      MAP_NEW(obj->dictv);

    MAP_INSERT(obj->dictv, key.s, key.len + 1, &value, sizeof(jsonval));
    ret = MAP_GET(obj->dictv, key.s, key.len + 1);

    stralloc_free(&key);

  } else if(obj->type == JSON_ARRAY) {
    int64 i = json_toint(name);
    int64 len = slist_size(obj->listv);

    if(len <= i) {
      while(len <= i) {
        if((ret = json_push(obj, json_undefined())))
          ++len;
      }
    } else {
      slink* link = obj->listv;
      for(len = 0; len < i; ++len) link = link->next;

      ret = slist_data(link);
    }
  }
  if(ret)
    *ret = value;

  return ret;
}
