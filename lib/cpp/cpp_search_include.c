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
      path[path_collapse(path, str_len(path))] = '\0';
      return path;
    }
  }

  return NULL;
}

char*
cpp_search_include_paths(cpp_ctx* pp, char* filename) {
  if(filename[0] == '/')
    return filename;

  char* cached = hashmap_get(&pp->inc_cache, filename);
  if(cached)
    return cached;

  size_t num_include_paths = strarray_size(&pp->inc_paths);

  /* Search a file from the include paths. */
  for(size_t i = 0; i < num_include_paths; i++) {
    char* path = path_join(strarray_AT(&pp->inc_paths, i), filename);

    if(!path_exists(path))
      continue;

    path[path_collapse(path, str_len(path))] = '\0';

    hashmap_put(&pp->inc_cache, filename, path);
    pp->inc_next_idx = i + 1;
    return path;
  }

  return NULL;
}
