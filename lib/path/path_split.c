#include "../path_internal.h"
#include "../utf8.h"
#include "../strlist.h"
#include <ctype.h>

int
path_split(const char* p, strlist* sl, char sep) {
  size_t i, j = 0;

  for(i = 0; p[i];) {
    size_t len = u8len(&p[i], 1);

    if(len == 1 && p[i] == sep) {
      strlist_pushb(sl, &p[j], i - j);
      i += len;
      j = i;
    }

    if(i > j)
      strlist_pushb(sl, &p[j], i - j);

    return strlist_count(sl);
  }

  /* size_t len = str_len(p);
   char sep = ':';

 #if WINDOWS
   if(len > 2 && isalnum(p[0]) && p[1] == ':' && path_issep(p[2]))
     sep = ';';
 #endif

   strlist_froms(sl, p, sep);

   return strlist_count(sl) > 1;*/
}
