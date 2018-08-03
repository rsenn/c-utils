#include "../hmap_internal.h"

size_t
hmap_size(HMAP_DB* my_hmap_db) {
  size_t ret = 0;
  if(my_hmap_db == NULL) return 0;
  TUPLE* ptr_list_tuple = my_hmap_db->list_tuple;
  while(ptr_list_tuple) {
    if(ptr_list_tuple->next == my_hmap_db->list_tuple)
      break;
    ptr_list_tuple = ptr_list_tuple->next;
    ++ret;
  }
  return ret;
}
