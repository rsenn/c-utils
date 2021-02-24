#include "../json_internal.h"

jsonval
json_object() {
  jsonval ret;
  ret.type = JSON_OBJECT;
  MAP_NEW(ret.dictv);
  return ret;
}