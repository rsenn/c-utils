#include "../stralloc.h"
#include "../path.h"

int
path_relative_to_sa(const stralloc* path,
                    const stralloc* relative_to,
                    stralloc* out) {
  return path_relative_to_b(
      path->s, path->len, relative_to->s, relative_to->len, out);
}
