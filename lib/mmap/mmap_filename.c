#include "../windoze.h"
#include "../mmap.h"
#include "../stralloc.h"
#include "../buffer.h"
#include "../scan.h"
#include "../open.h"

#if WINDOWS_NATIVE
#else
#include <unistd.h>
#include <limits.h>
#endif

int
mmap_filename(void* map, stralloc* sa) {
  buffer b = BUFFER_INIT(read, open_read("/proc/self/maps"), NULL, 0);
  char line[73 + PATH_MAX + 1];
  ssize_t n;

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

      buffer_puts(buffer_2, "mmap_filename: ");
      buffer_putxlonglong(buffer_2, start);
      buffer_puts(buffer_2, "-");
      buffer_putxlonglong(buffer_2, end);
      buffer_putnlflush(buffer_2);
    }
  }
}
