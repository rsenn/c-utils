#include "../hmap_internal.h"

/* hmap_add of the key and the value, both in strallocs; an error code if one is empty. */
int
hmap_set_stralloc(HMAP_DB** hmap_db, const stralloc* key, const stralloc* data) {
  if(!key->s || !key->len)
    return HMAP_KEY_EMPTY;

  if(!data->s || !data->len)
    return HMAP_FAILED;

  return hmap_add(hmap_db, key->s, key->len, 1, HMAP_DATA_TYPE_CHARS, data->s, data->len);
}
