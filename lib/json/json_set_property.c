#include "../hmap.h"
#include "../json.h"

jsonval*
json_set_property(jsonval* obj, jsonval name, jsonval value) {
  jsonval* ret = 0;
  if(obj->type == JSON_OBJECT) {
    stralloc key;
    stralloc_init(&key);
    json_tostring(name, &key);
    stralloc_nul(&key);

    /*    buffer_putm_internal(buffer_2, "key: ", key.s, "\n", 0);
        buffer_flush(buffer_2);*/

    if(obj->dictv == NULL)
      hmap_init(MAP_BUCKET, &obj->dictv);

    hmap_set(&obj->dictv, key.s, key.len + 1, &value, sizeof(jsonval));
    ret = hmap_get(obj->dictv, key.s, key.len + 1);

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
