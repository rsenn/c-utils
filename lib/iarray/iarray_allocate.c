#include <stdlib.h>
#include "../windoze.h"
#if !WINDOWS_NATIVE
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#endif
#include "../iarray.h"
#include "../cas.h"

static iarray_page*
new_page(size_t pagesize) {
#if WINDOWS_NATIVE
  void* x = malloc(pagesize);
  if(x == 0)
    return 0;
#else
  void* x = mmap(0, pagesize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if(x == MAP_FAILED)
    return 0;
#endif
  return (iarray_page*)x;
}

void*
iarray_allocate(iarray* ia, size_t pos) {
  size_t index, prevlen = ia->len;
  /* first, find the linked list of pages */
  iarray_page** p = &ia->pages[pos % (sizeof(ia->pages) / sizeof(ia->pages[0]))];
  /* this is here so we don't munmap and then re-mmap pages when a
   * certain path makes it necessary to mmap several pages into the
   * linked list and we are competing with another thread that does the
   * same thing */
  iarray_page* newpage = 0;
  /* since we have a fan-out of 16, on page 0 the elements are 0, 16, 32, ...
   * so we divide pos by the fan-out here */
  size_t realpos = pos;
  pos /= sizeof(ia->pages) / sizeof(ia->pages[0]);
  /* now walk the linked list of pages until we reach the one we want */
  for(index = 0;; index += ia->elemperpage) {
    if(!*p) {
      if(!newpage)
        if(!(newpage = new_page(ia->bytesperpage)))
          return 0;
      if(__CAS(p, 0, newpage) == 0)
        newpage = 0;
    }
    if(index + ia->elemperpage > pos)
      break;
    p = &(*p)->next;
  }
  if(newpage)
#if WINDOWS_NATIVE
    free(newpage);
#else
    munmap(newpage, ia->bytesperpage);
#endif
  {
    size_t l;
    size_t newlen = realpos + 1;
    do {
      l = __CAS(&ia->len, prevlen, newlen);
    } while(l < newlen);
  }
  return &(iarray_pagedata(*p)[(pos - index) * ia->elemsize]);
}
