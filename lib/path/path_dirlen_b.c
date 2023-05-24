#include "../path_internal.h"
#include "../byte.h"

size_t
path_dirlen_b(const char* path, size_t size) {
  return byte_rchrs(path, size, PATHSEP_S_MIXED, sizeof(PATHSEP_S_MIXED) - 1);
}
