#include "../json_internal.h"

jsonitem*
json_array_at(jsonval val, int64 pos) {
  jsonitem* item;

  item = json_array_items(val);
  while(pos-- > 0) {
    if(!(item = item->next))
      break;
  }

  return item;
}
