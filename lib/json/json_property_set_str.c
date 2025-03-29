#include "../uint64.h"
#include "../scan.h"
#include "../json_internal.h"

jsonval*
json_property_set_str(jsonval* obj, const char* key, jsonval value) {
  jsonval* ret = 0;

  if(obj->type == JSON_OBJECT) {
    buffer_putm_internal(buffer_2, "key: ", key, "\n", NULL);
    buffer_flush(buffer_2);

    if(MAP_ISNULL(obj->dictv))
      MAP_NEW(obj->dictv);
    MAP_INSERT2(obj->dictv, key, str_len(key), &value, sizeof(jsonval));
    ret = MAP_GET(obj->dictv, key, str_len(key));
  } else if(obj->type == JSON_ARRAY) {
    int64 i;
    int64 len = slist_size(obj->listv);

    if(len <= i) {
      while(len <= i) {
        if((ret = json_array_push(obj, json_undefined())))
          ++len;
      }
    } else {
      slink* link = obj->listv;

      for(len = 0; len < i; ++len)
        link = link->next;
      ret = slist_data(link);
    }
  }

  if(ret)
    *ret = value;

  return ret;
}
