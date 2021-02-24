#include "../json_internal.h"

jsonval
json_property_get_str(jsonval obj, const char* key) {
  jsonval ret = {.type = JSON_UNDEFINED}, *ptr;
  if(obj.type == JSON_OBJECT) {

    if((ptr = MAP_GET(obj.dictv, key, str_len(key)))) {
      ret = *ptr;
    }
  }
  return ret;
}
