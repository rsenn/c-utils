#include "../stralloc.h"
#include "../strlist.h"
#include "../byte.h"

void
strlist_intersection(const strlist* s1, const strlist* s2, strlist* out) {
  char *a = stralloc_begin(&s1->sa), *b = stralloc_begin(&s2->sa);
  char *aend = stralloc_end(&s1->sa), *bend = stralloc_end(&s2->sa);
  while(a < aend && b < bend) {
    size_t alen = byte_chr(a, aend - a, s1->sep);
    size_t blen = byte_chr(b, bend - b, s2->sep);

    int delta = byte_diff2(a, alen, b, blen);
    if(delta < 0) {
      a += alen + 1;
    } else if(delta > 0) {
      b += blen + 1;
    } else {
      // A[i] == B[j]
      strlist_pushb(out, a, alen);
      a += alen + 1;
      b += blen + 1;
    }
  }
}
