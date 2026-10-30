#include "../ini.h"

/* ini_keys returns the keys of the section as a strarray */
strarray
ini_keys(ini_section_t* ini) {
  MAP_PAIR_T el;
  strarray ret;
  strarray_init(&ret);

  for(el = ini->map.buckets; el < &ini->map.buckets[ini->map.capacity]; ++el)
    if(el->key && el->key != HASHMAP_TOMBSTONE)
      strarray_push(&ret, MAP_KEY(el));

  return ret;
}
