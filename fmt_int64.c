#include "fmt.h"

size_t fmt_int64(char *dest,int64 i) {
  if(i<0) {
    if(dest) *dest++='-';
    return fmt_uint64(dest,(uint64)-i)+1;
  } else
    return fmt_uint64(dest,(uint64)i);
}

