#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include "../byte.h"
#include "../ip6.h"
#include "../scan.h"
#include <ctype.h>

/* scan_ip6 plus an optional "%interface" (a name or a number) giving the scope id.
 * The interface name used to be freed even when it pointed into the input string. */
size_t
scan_ip6if(const char* src, char* ip, uint32* scope_id) {
  size_t i = scan_ip6(src, ip);

  *scope_id = 0;
  if(src[i] == '%') {
    size_t j, n;
    char name[32];

    for(j = i + 1; isalnum((unsigned char)src[j]); ++j)
      ;
    n = j - (i + 1);
    if(n > 0 && n < sizeof name) {
      byte_copy(name, n, &src[i + 1]);
      name[n] = 0;
      if(isdigit((unsigned char)name[0])) {
        unsigned long idx = 0;

        scan_ulong(name, &idx);
        *scope_id = (uint32)idx;
      } else {
        *scope_id = socket_getifidx(name);
      }
      i = j;
    }
  }
  return i;
}
