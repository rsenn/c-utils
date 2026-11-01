#include "../cpp.h"
#include "../cpp_internal.h"
#include "../strarray.h"
#include "../path.h"

char*
cpp_search_include_next(cpp_ctx* pp, char* filename) {
  size_t num_include_paths = strarray_size(&pp->inc_paths);

  for(; pp->inc_next_idx < num_include_paths; pp->inc_next_idx++) {
    char* path = path_join(strarray_AT(&pp->inc_paths, pp->inc_next_idx), filename);

    if(path_exists(path)) {
      path_canon(path, str_len(path) + 2, path, str_len(path));
      return path;
    }
  }

  return NULL;
}
