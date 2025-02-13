#include "../path_internal.h"
#include "../utf8.h"

int
path_getsep(const char* path) {
  size_t i, len;

  for(i = 0; path[i]; i += len)
    if((len = u8_len(&path[i], 1)) == 1 && path_issep(path[i]))
      return path[i];

  /*  while(*path) {
      if(path_issep(*path))
        return *path;
      ++path;
    }*/

  return '\0';
}
