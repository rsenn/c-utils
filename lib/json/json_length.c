#include "../json.h"

int64
json_length(jsonval v) {
  switch(v.type) {
    case JSON_STRING: {
      return v.stringv.len;
    }
    case JSON_ARRAY: {
      return slist_length(&v.listv);
    }
    case JSON_OBJECT: {
      return MAP_SIZE(v.dictv);
    }
    default: break;
  }
  return 0;
}
