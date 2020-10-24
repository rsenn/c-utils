#include <stdlib.h>
#ifn WINDOWS_NATIVE
#include <sys/mman.h>
#endif
#include <unistd.h>
#include "../iarray.h"

static void
freechain(iarray_page* p, size_t pagesize) {
  while(p) {
    iarray_page* n = p->next;
#if WINDOWS_NATIVE
    free(p);
#else
    munmap(p, pagesize);
#endif
    p = n;
  }
}

void
iarray_free(iarray* ia) {
  size_t i;
  for(i = 0; i < sizeof(ia->pages) / sizeof(ia->pages[0]); ++i) {
    freechain(ia->pages[i], ia->bytesperpage);
    ia->pages[i] = 0;
  }
}
