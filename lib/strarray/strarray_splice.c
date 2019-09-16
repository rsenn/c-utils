#include "../strarray.h"
#include "../array.h"
#include "../byte.h"
#include "../safemult.h"
#include "../uint64.h"
#include "../str.h"
#include "../alloc.h"

int64
strarray_splice(strarray* a, uint64 start, uint64 del, uint64 insert, const char** x) {
  char** s = array_get(a, sizeof(char*), start);
  uint64 i, len, newlen;

  len = strarray_size(a);
  
  if(start + del > len)
    del = len - start;

  for(i = 0; i < del; i++)
    alloc_free(s[i]);

  newlen = len - del + insert;

  if(insert != del) {
    char **end, **move = s + del;
    len = a->p + a->initialized - (char*)move;
   
    if(insert > del) {
      end = array_allocate(a, sizeof(char*), newlen);
      s = end - newlen + start;
      move = s + del;
    } else {
      end = array_end(a);
    }

    if(len) {
      (insert > del ? byte_copyr : byte_copy)((char*)end - len, len, move);
    }
  }

  for(i = 0; i < insert; ++i) 
    s[i] = str_dup(x[i]);

  array_truncate(a, sizeof(char*), newlen);

  return newlen;
}

