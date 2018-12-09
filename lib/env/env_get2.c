/* ISC license. */

#include "../env.h"
#include "../str.h"
#include <string.h>

const char*
env_get2(const char* const* envp, const char* s) {
  size_t len, i;
  if(!s)
    return 0;
  len = str_len(s);
  for(i = 0; envp[i]; ++i)
    if(str_start(envp[i], s) && (envp[i][len] == '='))
      return envp[i] + len + 1;
  return 0;
}
