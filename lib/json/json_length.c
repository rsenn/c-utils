#include "../json_internal.h"

int64
json_length(jsonval v) {
  switch(v.type) {
    case JSON_STRING: return v.stringv.len;
    case JSON_ARRAY: {
      jsonitem* item;
      uint64 n = 0;
      for(item = json_array_items(v); item; item = item->next)
        n++;
      return n;
    }
    case JSON_OBJECT: return MAP_SIZE(v.dictv);
  }
  return 0;
}
