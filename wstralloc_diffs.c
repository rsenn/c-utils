#include "wstralloc.h"
#include "byte.h"
#include "wstr.h"

extern int wstralloc_diffs(const wstralloc* a, const wchar_t* b) {
  register size_t i;
  register int j;
  for(i = 0;;++i) {
    if(i == a->len) return(!b[i]) ? 0:-1; if(!b[i]) return 1;
    if((j = ((unsigned short)(a->s[i]) - (unsigned short)(b[i])))) return j;
  }
  return j;
}

