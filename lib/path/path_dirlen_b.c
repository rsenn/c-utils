#include "../path_internal.h"
#include "../byte.h"

size_t
path_dirlen_b(const char* path, size_t size) {
  const char ps[] = PATHSEP_S_MIXED;
  const size_t pslen = sizeof(ps) - 1;

  return byte_rchrs(path, size, ps, pslen);
}
