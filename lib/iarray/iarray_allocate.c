#ifdef __POCC__
#define NOWINBASEINTERLOCK 1
#define _NTOS_ 1
long __stdcall InterlockedCompareExchange(long volatile *,long,long);
void* __stdcall InterlockedCompareExchangePointer(void* volatile *,void*,void*);
#endif

#include "../windoze.h"
#include "../byte.h"
#include "../iarray.h"

#if defined(__STDC__) 
#if __STDC_VERSION__ >= 201112L  && !defined(__EMSCRIPTEN__) && !defined(__ANDROID__)
#include <stdatomic.h>
#endif
#endif

#include <fcntl.h>

#include "../cas.h"

#if !WINDOWS_NATIVE
#include <unistd.h>
#include <sys/mman.h>
#endif

#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS 0x20

#endif

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
  byte_zero(x, pagesize);
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
      if(__CAS_PTR((void**)p, 0, (void*)newpage) == 0)
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
    do {
      l = (size_t)__CAS(&ia->len, prevlen, realpos);
    } while(l < realpos);
  }
  return &iarray_data(*p)[(pos - index) * ia->elemsize];
}
