#include <stdlib.h>
#include <errno.h>
#include "../alloc.h"

#ifdef DJB_STRICT

#define ALIGNMENT 16 /* XXX: assuming that this alignment is enough */
#define SPACE 2048   /* must be multiple of ALIGNMENT */

typedef union {
  char irrelevant[ALIGNMENT];

  double d;
} aligned;

static aligned realspace[SPACE / ALIGNMENT];
#define space ((char*)realspace)
static unsigned int avail = SPACE; /* multiple of ALIGNMENT; 0<=avail<=SPACE */

void*
alloc(size_t n) {
  char* x;
  n = ALIGNMENT + n - (n & (ALIGNMENT - 1)); /* XXX: could overflow */

  if(n <= avail) {
    avail -= n;
    return space + avail;
  }
  x = malloc(n);

  if(!x)
    errno = ENOMEM;
  return x;
}

void
alloc_free(void* x) {
  if(x >= space)

    if(x < space + SPACE)
      return; /* XXX: assuming that pointers are flat */
  free(x);
}
#else

void*
alloc(size_t n) {
  return malloc(n);
}
void
alloc_free(void* ptr) {
  free(ptr);
}

#endif
