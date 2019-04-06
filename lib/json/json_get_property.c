#include "../json.h"

jsonval*
json_get_property(jsonval* obj, jsonval name) {
  jsonval* ret = 0;
  if(obj->type == JSON_OBJECT) {
    stralloc key;
    stralloc_init(&key);
    json_tostring(&name, &key);
    stralloc_nul(&key);
    ret = hmap_get(&obj->dictv, key.s, key.len);
    stralloc_free(&key);
  } else if(obj->type == JSON_ARRAY) {
    struct slink* p;
    int64 index = json_toint(name);

    for(p = obj->listv; p; p = p->next) {
      if(index-- == 0)
        ret = slist_data(p);
        break;
      }
    }
  }
  return ret;
}

