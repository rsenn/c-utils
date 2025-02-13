#include "../path.h"
#include "../utf8.h"

#if WINDOWS_NATIVE
#include <shlwapi.h>
#else
#include <sys/stat.h>
#endif

int
path_is_root(const char* p) {
#if WINDOWS_NATIVE
  size_t len = u8swcslen(p);
  wchar_t* w = alloc((len + 1) * sizeof(wchar_t));
  u8stowcs(w, p, len);
  w[len] = '\0';

  return PathIsRootW(w);
#else
  struct stat st[2];
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, p);
  stralloc_cats(&sa, "/..");
  stralloc_nul(&sa);

  if(lstat(p, &st[0]) == -1)
    return -1;
  if(lstat(sa.s, &st[1]) == -1)
    return -1;

  stralloc_free(&sa);

  return st[0].st_dev == st[1].st_dev && st[0].st_ino == st[1].st_ino;
#endif
}
