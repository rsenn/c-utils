
#if 0
#define MAP_T cbmap_t
#define MAP_SIZE cbmap_count
#define MAP_NEW(map) map = cbmap_new()
#define MAP_VISIT_ALL cbmap_visit_all
//#define MAP_GET(map, key, klen) cbmap_get
static inline void* MAP_GET(cbmap_t map, const void* key, size_t klen) {
  void* ret = NULL;
  size_t rlen = 0;
  cbmap_get(map, key, klen, &ret, &rlen);
  return (ret && rlen) ? ret : NULL;
}

#define MAP_INSERT cbmap_insert
#else
typedef HMAP_DB* MAP_T;
#define MAP_SIZE hmap_size
#define MAP_NEW(map) hmap_init(MAP_BUCKET, &(map))
#define MAP_DESTROY(map) hmap_destroy(&(map))
#define MAP_VISIT_ALL(map, fn, arg)                                                                                                                                                                    \
  {                                                                                                                                                                                                    \
    TUPLE* t;                                                                                                                                                                                          \
    hmap_foreach(map, t) fn(t->key, t->key_len, t->vals.val_chars, t->data_len, arg);                                                                                                                  \
  }
#define MAP_GET(map, key, klen) hmap_get(map, key, klen)
/*static inline void*
MAP_GET(HMAP_DB* map, const void* key, size_t klen) {
  TUPLE* t = NULL;
  if(hmap_search(map, key, klen, &t) == HMAP_SUCCESS) {
    if(t->data_type == HMAP_DATA_TYPE_CUSTOM)
      return t->vals.val_custom;

    return t->vals.val_chars;
  }

  return NULL;
}*/

#define MAP_DELETE(map, key, klen) hmap_delete(&(map), key, klen)

#define MAP_INSERT(map, key, klen, data, dlen) (dlen == 0 ? hmap_add(&(map), key, klen, 0, HMAP_DATA_TYPE_CUSTOM, data) : hmap_set(&(map), key, klen, data, dlen))
#endif

#if WINDOWS
#define MAX_CMD_LEN 1023

#define DEFAULT_OBJEXT ".obj"
#define DEFAULT_LIBEXT ".lib"
#define DEFAULT_LIBPFX ""
#define DEFAULT_EXEEXT ".exe"
#define DEFAULT_PPSEXT ".pp.c"
#else
#define MAX_CMD_LEN 8191

#define DEFAULT_OBJEXT ".o"
#define DEFAULT_LIBEXT ".a"
#define DEFAULT_LIBPFX "lib"
#define DEFAULT_EXEEXT ""
#define DEFAULT_PPSEXT ".pp.c"
#endif

#if WINDOWS_NATIVE
#define DEFAULT_PATHSEP '\\'
#else
#define DEFAULT_PATHSEP '/'
#endif