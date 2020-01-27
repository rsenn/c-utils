#include "lib/windoze.h"
#include "lib/unix.h"
#include "lib/hmap.h"
#include "lib/mmap.h"
#include "lib/open.h"
#include "lib/path.h"
#include "lib/rdir.h"
#include "lib/scan.h"
#include "lib/slist.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/strarray.h"
#include "lib/strlist.h"
#include "lib/uint32.h"
#include "lib/errmsg.h"
#include "lib/array.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/dir.h"
#include "lib/range.h"
#include "lib/case.h"


typedef enum { WIN, MAC, LINUX } os_type;

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
#define MAP_VISIT_ALL(map, fn, arg)                                                                                    \
  {                                                                                                                    \
    TUPLE* t;                                                                                                          \
    hmap_foreach(map, t) fn(t->key, t->key_len, t->vals.val_chars, t->data_len, arg);                                  \
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

#define MAP_INSERT(map, key, klen, data, dlen)                                                                         \
  (dlen == 0 ? hmap_add(&(map), key, klen, 0, HMAP_DATA_TYPE_CUSTOM, data) : hmap_set(&(map), key, klen, data, dlen))
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

typedef struct {
  enum { X86, ARM, PIC } arch;
  enum { _14, _16, _32, _64 } bits;
} machine_type;

typedef struct {
  os_type os;
  enum { NTOS, UNIX } type;
} system_type;

typedef struct {
  struct slink link;
  const char* name;
  int has_main;
} sourcefile;

typedef struct {
  int n_sources;
  slink* sources;
  strlist includes;
  array rules;
} sourcedir;

typedef struct {
  const char* name;
  strlist output;
  strlist prereq;
  stralloc recipe;
  array deps;
  array objs;
  uint32 serial;
} target;

typedef struct {
  strlist work;
  strlist build;
  strlist out;
} dirs_t;

typedef struct {
   char* obj;
  char* lib;
  char* bin;
  char* pps;
} exts_t;

enum {
  BUILD_TYPE_RELEASE = 0,
  BUILD_TYPE_RELWITHDEBINFO,
  BUILD_TYPE_MINSIZEREL,
  BUILD_TYPE_DEBUG,
};

void debug_int(const char* name, int i);
void debug_sa(const char* name, stralloc* sa);
void debug_sl(const char* name, const strlist* l);
void debug_s(const char* name, const char* s);