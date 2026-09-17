#include "../str.h"
#include "../strlist.h"
#include <stdlib.h>

void
strlist_push_tokens(strlist* sl, const char* s, const char* delim) {
  size_t p, n, len = str_len(delim);

  for(p = 0; s[p]; p += n + len) {
    n = str_find(&s[p], delim);
    strlist_pushb(sl, &s[p], n);

    /* str_find() returns str_len(&s[p]) (i.e. n == the remaining
     * length) when delim isn't found -- the last token, with no
     * trailing delimiter, is the normal case. Advancing past it by
     * "+ len" anyway would move p beyond the string's NUL terminator,
     * and the loop condition would then read out of bounds. */
    if(s[p + n] == '\0')
      break;
  }
}
