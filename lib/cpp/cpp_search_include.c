#include "../cpp.h"
#include "../cpp_internal.h"
#include "../strarray.h"
#include "../path.h"


char*
cpp_search_include_next(char* filename) {
  size_t num_include_paths = strarray_size(&cpp_include_paths);

  for(; cpp_include_next_idx < num_include_paths; cpp_include_next_idx++) {
    char* path = cpp_format("%s/%s", strarray_AT(&cpp_include_paths, cpp_include_next_idx), filename);

    if(path_exists(path)) {
      path[path_collapse(path, str_len(path))] = '\0';
      return path;
    }
  }

  return NULL;
}

char*
cpp_search_include_paths(char* filename) {
  if(filename[0] == '/')
    return filename;

  static hashmap cache;
  char* cached = hashmap_get(&cache, filename);
  if(cached)
    return cached;

  size_t num_include_paths = strarray_size(&cpp_include_paths);

  /* Search a file from the include paths. */
  for(size_t i = 0; i < num_include_paths; i++) {
    char* path = cpp_format("%s/%s", strarray_AT(&cpp_include_paths, i), filename);

    if(!path_exists(path))
      continue;

    path[path_collapse(path, str_len(path))] = '\0';

    hashmap_put(&cache, filename, path);
    cpp_include_next_idx = i + 1;
    return path;
  }

  return NULL;
}
