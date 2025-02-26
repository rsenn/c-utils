/* ISC license. */

#include "../env.h"
#include "../stralloc.h"

int
env_string(stralloc* sa, const char* const* envp, size_t envlen) {
  size_t salen = sa->len;
  size_t i = 0;

  for(; i < envlen; i++) {
    if(!stralloc_cats(sa, envp[i]) || !stralloc_0(sa)) {
      sa->len = salen;
      return 0;
    }
  }
  return 1;
}
