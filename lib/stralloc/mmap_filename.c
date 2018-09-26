#include "../windoze.h"
#include "../mmap.h"
#include "../stralloc.h"
#include "../buffer.h"
#include "../scan.h"
#include "../open.h"

#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

int
mmap_filename(void* map, stralloc* sa) {
#if WINDOWS_NATIVE
  stralloc_ready(sa, MAX_PATH+1);
  sa->len = GetMappedFileNameA(GetCurrentProcess(), map, sa->s, sa->a);
  return sa->len > 0;
#else
  char buf[1024];
  buffer b = BUFFER_INIT(read, open_read("/proc/self/maps"), buf, sizeof(buf));
  char line[73 + PATH_MAX + 1];
  ssize_t n;
  int ret = 0;

  while((n = buffer_getline(&b, line, sizeof(line))) > 0) {
    char* p = line;
    unsigned long long start, end;

    p += scan_xlonglong(p, &start);
    if(*p == '-') {
      char* e = line + n;
      int i = 4;
      ++p;
      p += scan_xlonglong(p, &end);

      while(i--) {
        p += scan_whitenskip(p, e - p);
        p += scan_nonwhitenskip(p, e - p);
      }
      p += scan_whitenskip(p, e - p);

      if((unsigned long long)map >= start && (unsigned long long)map < end) {
        stralloc_copyb(sa, p, e - p);
        ret = 1;
        break;
      }
    }
  }

  buffer_close(&b);
  return ret;
#endif
}
