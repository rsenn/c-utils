#include "../json_internal.h"

jsonitem*
json_array_items(jsonval val) {
  if(val.type == JSON_ARRAY)
    return val.itemv;
  return 0;
}
