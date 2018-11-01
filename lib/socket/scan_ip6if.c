#define USE_WS2_32 1

#if WINDOWS
#define _WINSOCKAPI_
#include <winsock2.h>
#endif

#include "../socket_internal.h"
#include "../byte.h"
#include "../ip6.h"
#include <stdlib.h>
#include <ctype.h>

size_t
scan_ip6if(const char* src, char* ip, uint32* scope_id) {
  size_t i = scan_ip6(src, ip);
  *scope_id = 0;
  if(src[i] == '%') {
    size_t j;
    char* tmp = 0;
    for(j = i + 1; isalnum(src[j]); ++j)
      ;
    if(!src[j])
      tmp = (char*)src + i + 1;
    else {
      tmp = malloc(j - i);
      byte_copy(tmp, j - (i + 1), src + i + 1);
      tmp[j - (i + 1)] = 0;
    }
    if(tmp) {
    if(*tmp) {
      *scope_id = socket_getifidx(tmp);
      i = j;
    }
    free(tmp);
    }
  }
  return i;
}
