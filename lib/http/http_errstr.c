#include "../http.h"
#include "../str.h"

#include <string.h>

size_t
http_errstr(int errnum, char* buf, size_t buflen) {
  size_t ret;
#ifdef HAVE_STRERROR_R
  ret = strerror_r(errnum, buf, buflen) ? 0 : str_len(buf);
#else
  ret = str_copyn(buf, strerror(errnum), buflen);
#endif
  return ret;
}
