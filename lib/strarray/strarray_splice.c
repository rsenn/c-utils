#include "../strarray.h"
#include "../array.h"
#include "../byte.h"
#include "../safemult.h"
#include "../uint64.h"

int64
strarray_splice(strarray* a, uint64 start, uint64 del, uint64 insert, const char** x) {
  char** s = &strarray_at(a, start);
  uint64 i, len, newlen;

  len = strarray_size(a);
  
  if(start + del > len)
    del = len - start;

  if(insert) { 
    if(!umult64(sizeof(char*), insert, &i))
      return -1;
  }

  for(i = 0; i < del; i++)
    free(s[i]);

  newlen = len - del + insert;

  if(insert != del) {
    char** move = strarray_at(a, start + del);
    len = a->p + a->initialized - (char*)move;

    (insert > del ? byte_copyr : byte_copy)((char*)array_allocate(a, sizeof(char*), newlen) - len, len, move);
  }

  while(insert-- > 0)
    *s++ = str_dup(*x++);

  array_truncate(a, sizeof(char*), newlen);

  return newlen;
}
