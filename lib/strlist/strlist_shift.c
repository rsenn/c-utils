#include "../byte.h"
#include "../str.h"
#include "../strlist.h"

int
strlist_shift(strlist* sl, const char** strp) {
  size_t offs;
  if(sl->sa.len == 0) return 0;

  offs = strlist_at(sl, 1) - sl->sa.s;

  sl->sa.s[offs - 1] = '\0';

  *strp = str_ndup(sl->sa.s, offs - 1);

  byte_copy(sl->sa.s, sl->sa.len - offs, &sl->sa.s[offs]);
  sl->sa.len -= offs;
  return 1;
}
