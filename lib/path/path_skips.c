#include "../path_internal.h"
#include "../utf8.h"

size_t
path_skips(const char* s) {
  size_t i, len;

  /* skip one path component (a run of non-separator chars) ... */
  for(i = 0; s[i]; i += len) {
    /* u8_len() returns 0 on an invalid UTF-8 lead byte -- treat it
     * as one raw byte instead of looping forever without advancing. */
    if((len = u8_len(&s[i], 1)) == 0)
      len = 1;

    if(len == 1 && path_issep(s[i]))
      break;
  }

  /* ... plus its trailing separator run -- continuing from where the
   * loop above left off, not resetting to 0, so the two runs add up
   * cumulatively (as callers doing repeated `p += path_skips(p)`
   * expect), not just the separator run on its own. */
  for(; s[i]; i += len)
    if((len = u8_len(&s[i], 1)) > 1 || !path_issep(s[i]))
      break;

  return i;
}
