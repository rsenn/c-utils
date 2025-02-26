#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include "../fmt.h"
#include "../ip6.h"
#include "../str.h"

size_t
fmt_ip6if(char* dest, const char* ip, uint32 scope_id) {
  size_t i = fmt_ip6(dest, ip);

  if(scope_id) {
    if(dest) {
      dest[i] = '%';
      ++i;
      dest += i;
    }
    i += fmt_str(dest, socket_getifname(scope_id));
  }
  return i;
}
