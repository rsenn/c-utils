#include "../scan.h"
#include <ctype.h>

/* Length of the ANSI escape sequence (ESC [ params letter [~]) at the start of
 * s[0..limit), 0 if there is none or if it is cut off by limit. */
size_t
scan_ansiskip(const char* s, size_t limit) {
  size_t i;

  if(limit < 2 || s[0] != 0x1b || s[1] != '[')
    return 0;

  for(i = 2; i < limit; ++i)
    if(isalpha((unsigned char)s[i]))
      break;

  if(i >= limit)
    return 0; /* incomplete */

  ++i;
  if(i < limit && s[i] == '~')
    ++i;
  return i;
}
