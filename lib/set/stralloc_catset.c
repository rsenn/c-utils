#include "../set.h"
#include "../str.h"
#include "../stralloc.h"

size_t
stralloc_catset(stralloc* out, const set_t* set, const char* separator) {
  char* x;
  size_t i = 0, n, slen = str_len(separator);
     stralloc_cats(sa, "{ ");
   set_foreach(set, x, n) {
    if(i++ > 0) {
         stralloc_catb(sa, slen, separator);
      i += slen;
    }
       stralloc_catb(sa, n, x);
    i += n;
  }
   
    stralloc_cats(sa, " }\n");
  i += 3;
  return i;
}
