#include "../windoze.h"
#include "../dir_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include <assert.h>

#if USE_WIDECHAR
#include "../utf8.h"
#endif

#if !USE_READDIR
#include <stdio.h>
#endif

static const char* last_error_str();

int
dir_open(struct dir_s* d, const char* p)
{
  int ret;
  if(!(d->dir_int = malloc(sizeof(struct dir_internal_s)))) return 1;
#if USE_READDIR
  ret = ((dir_INTERNAL(d)->dir_handle = opendir(p)) == NULL) ? -1 : 0;
#else
  {
    stralloc path;
    HANDLE h;
    stralloc_init(&path);
    stralloc_copys(&path, p);
    
   if(path.len > 0 && path.s[path.len - 1] != '\\')
   stralloc_catc(&path, '\\');
   stralloc_catc(&path, '*');
   stralloc_nul(&path);

   dir_INTERNAL(d)->dir_path = path.s;

#if USE_WIDECHAR
  {
    size_t wlen = u8swcslen(path.s);
      wchar_t* wpath = malloc((wlen + 1) * sizeof(wchar_t));

      u8stowcs(wpath, path.s, wlen);
      free(path.s);

      wpath[wlen] = '\0';
      dir_INTERNAL(d)->dir_path = wpath;
      h = FindFirstFileW(wpath, &dir_INTERNAL(d)->dir_finddata);
    }
    dir_INTERNAL(d)->tmpname = NULL;
#else
    h = FindFirstFileA(path.s, &dir_INTERNAL(d)->dir_finddata);
#endif
    dir_INTERNAL(d)->first = 1;
    dir_INTERNAL(d)->dir_handle = h;
    ret = (h == INVALID_HANDLE_VALUE);

    /*  if(ret) {
       DWORD error = GetLastError();
       printf("ERROR: %s\n", last_error_str());
    }*/
  }
#endif

  /* assert(dir_INTERNAL(d)->dir_handle); */

  return ret;
}

#if !USE_READDIR
static const char*
last_error_str()
{
  DWORD errCode = GetLastError();
  static char tmpbuf[1024];
  char* err;
  tmpbuf[0] = '\0';
  if(errCode == 0) return tmpbuf;
  SetLastError(0);
  if(!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL,
                    errCode,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), /* default language */
                    (LPTSTR)&err,
                    0,
                    NULL))
    return 0;
  snprintf(tmpbuf, sizeof(tmpbuf), "ERROR: %s\n", err);
  /* or otherwise log it */
  // OutputDebugString(tmpbuf);
  LocalFree(err);
  return tmpbuf;
}
#endif
