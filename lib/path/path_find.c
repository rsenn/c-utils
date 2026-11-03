#include "internal.h"
#include "../dirlist.h"
#include "../utf8.h"

int
path_find(const char* path, const char* name, stralloc* out) {
  arena ar;
  struct dirlist dl;
  const char* s;
  char* rec;
  int ret = 0;

  arena_init(&ar, &arena_heap, 0);
  dl = dirlist_read(&ar, path, DIRLIST_DOTS);

  if(!dl.s) {
    arena_free(&ar);
    return 0;
  }

  dirlist_foreach(rec, dl) {
    s = dirlist_name(rec);

    if(!u8s_diff(s, name)) {
      if(*path && u8s_diff(path, ".")) {
        stralloc_copys(out, path);
        stralloc_catc(out, PATHSEP_C);
      }

      stralloc_cats(out, s);
      ret = 1;
      break;
    }
  }

  arena_free(&ar);
  return ret;
}
