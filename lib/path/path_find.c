#include "../path_internal.h"
#include "../dir.h"
#include "../utf8.h"

int
path_find(const char* path, const char* name, stralloc* out) {
  dir_t dir;
  const char* s;
  int ret = 0;

  if(dir_open(&dir, path))
    return 0;

  while((s = dir_read(&dir))) {
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

  dir_close(&dir);
  return ret;
}
