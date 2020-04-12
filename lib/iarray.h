/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef IARRAY_H
#define IARRAY_H

/* This header defines an indirect array for use with the io_* routines.
 * Assumptions:
 * - the elements are small (many fit on one page),
 * - the platform has an atomic compare-and-swap instruction
 * - the compiler supports it via __sync_val_compare_and_swap
 */

#include "uint64.h"
#include "typedefs.h"
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __BORLANDC__
#define iarray_data(page) ((char*)(page) + sizeof(iarray_page*))
#else
#define iarray_data(page) (page)->data
#endif

/* The basic data structure is a static array of pointers to pages.
 * Each page also contains a next pointer to form a linked list.
 * To get to element n, you take n % the number of elements in the
 * static array (iarray->pages) to get to the list of pages that
 * contains it. Then keep going to the next page until you are on the
 * right page.
 * Note: The elements on each page are not contiguous. If the fanout is
 * 16, the indices on page 0 are 0, 16, 32, ...
 * To get to element 0, you'd go to iarray->pages[0].data,
 * to get to element 1, you'd go to iarray->pages[1].data,
 * to get to element 16, you'd go to iarray->pages[0].data+iarray->elemsize.
 */
typedef struct _iarray_page {
  struct _iarray_page* next;
#if !defined(__BORLANDC__) && !defined(__ICL__)
  char data[0];
#endif
} iarray_page;

typedef struct {
  iarray_page* pages[16];
  size_t elemsize, elemperpage, bytesperpage;
#ifndef __POCC__
  volatile
#endif
      unsigned long len;
} iarray;

void*  iarray_allocate(iarray*, size_t pos);
void*  iarray_get(iarray*, size_t pos);
void   iarray_init(iarray*, size_t elemsize);
size_t iarray_length(iarray*);

/* WARNING: do not use the array during or after iarray_free, make sure
 * no threads are potentially doing anything with the iarray while it is
 * being freed! */
void   iarray_free(iarray*);

#ifdef __cplusplus
}
#endif
#endif
