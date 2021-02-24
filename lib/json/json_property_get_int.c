#include "../json_internal.h"

jsonval
json_property_get_int(jsonval obj, int64 i) {
  jsonval ret = {.type = JSON_UNDEFINED};
  if(obj.type == JSON_ARRAY) {
    struct slink* p;
    for(p = obj.listv; p; p = p->next) {
      if(i-- == 0) {
        ret = *(jsonval*)slist_data(p);
        break;
      }
    }
  }
  return ret;
}