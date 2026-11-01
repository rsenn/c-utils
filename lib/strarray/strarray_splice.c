#include "../alloc.h"
#include "../array.h"
#include "../byte.h"
#include "../str.h"
#include "../strarray.h"

/* Removes del strings at start (and frees them), puts copies of the insert
 * strings x[0..insert) in their place and returns the new size, -1 when out of
 * memory (the array is then unchanged). start and del are clipped to the size. */
int64
strarray_splice(strarray* a, uint64 start, uint64 del, uint64 insert, const char** x) {
  size_t len = strarray_size(a), i;
  size_t newlen;
  char **dup = NULL, **s;

  if(start > len)
    start = len;
  if(del > len - start)
    del = len - start;
  newlen = len - del + insert;

  /* copy the new strings first: nothing is touched if that fails */
  if(insert) {
    if(!(dup = (char**)alloc(insert * sizeof(char*))))
      return -1;
    for(i = 0; i < insert; ++i) {
      if(!(dup[i] = str_dup(x[i]))) {
        while(i > 0)
          alloc_free(dup[--i]);
        alloc_free(dup);
        return -1;
      }
    }
  }

  if(newlen > len && !array_allocate(&a->a, sizeof(char*), newlen - 1)) {
    for(i = 0; i < insert; ++i)
      alloc_free(dup[i]);
    alloc_free(dup);
    return -1;
  }

  s = strarray_BEGIN(a);
  for(i = 0; i < del; ++i)
    alloc_free(s[start + i]);

  if(insert != del && len > start + del)
    byte_copyr(&s[start + insert], (len - start - del) * sizeof(char*), &s[start + del]);
  for(i = 0; i < insert; ++i)
    s[start + i] = dup[i];
  if(dup)
    alloc_free(dup);

  if(newlen < len)
    array_truncate(&a->a, sizeof(char*), newlen);
  return (int64)newlen;
}
