#include "../cpp.h"
#include "../cpp_internal.h"
#include "../strarray.h"
#include "../path.h"

char*
cpp_search_include_paths(cpp_ctx* pp, char* filename) {
  char* cached;
  size_t num_include_paths;
  size_t i;
  if(filename[0] == '/')
    return filename;

  cached = hashmap_get(&pp->inc_cache, filename);
  if(cached)
    return cached;

  num_include_paths = strarray_size(&pp->inc_paths);

  /* Search a file from the include paths. */
  for(i = 0; i < num_include_paths; i++) {
    char* path = path_join(strarray_AT(&pp->inc_paths, i), filename);

    if(!path_exists(path))
      continue;

    path_canon(path, str_len(path) + 2, path, str_len(path));

    hashmap_put(&pp->inc_cache, filename, path);
    pp->inc_next_idx = i + 1;
    return path;
  }

  return NULL;
}
