#include "../strarray.h"
#include "../array.h"
#include "../byte.h"
#include "../safemult.h"
#include "../uint64.h"
#include "../str.h"
#include "../alloc.h"

int64
strarray_splice(
    strarray* a, uint64 start, uint64 del, uint64 insert, const char** x) {
  char** s;
  uint64 i, len, newlen;
  if(strarray_begin(a) == NULL) {
    array_allocate(a, sizeof(char*), 0);
    array_trunc(a);
  }
  s = (char**)array_get(a, sizeof(char*), start);
  len = strarray_size(a);
  if(start + del > len)
    del = len - start;
  for(i = 0; i < del; i++) alloc_free(s[i]);
  newlen = len - del + insert;
  if(insert != del) {
    size_t nmove = len - (start + del);
    size_t movepos = &s[i] - strarray_begin(a);
    char** end = strarray_end(a);
    if(insert > del) {
      end = (char**)array_allocate(a, sizeof(char*), newlen);
      s = end - newlen + start;
      // move = s + del;
    }
    if(nmove) {
      (insert > del ? byte_copyr
                    : byte_copy)(&s[insert],
                                 nmove * sizeof(char*),
                                 array_get(a, sizeof(char*), movepos));
    }
  }
  for(i = 0; i < insert; ++i) s[i] = str_dup(x[i]);
  array_truncate(a, sizeof(char*), newlen);
  return newlen;
}
