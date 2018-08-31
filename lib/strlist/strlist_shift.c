#include "../str.h"
#include "../byte.h"
#include "../strlist.h"

int
strlist_shift(strlist* sl, const char** strp) {
  size_t offs;
  if(sl->sa.len == 0) return 0;

  offs = byte_chr(sl->sa.s, sl->sa.len, sl->sep);

  *strp = str_ndup(sl->sa.s, offs);

  byte_copyr(sl->sa.s, sl->sa.len - offs, &sl->sa.s[offs]);
  sl->sa.len -= offs;
  return 1;
}
