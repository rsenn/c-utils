#include "../cpp.h"
#include "../cpp_internal.h"
#include "../strarray.h"

char*
cpp_search_include_paths(char* filename) {
  if(filename[0] == '/')
    return filename;

  static hashmap cache;
  char* cached = hashmap_get(&cache, filename);
  if(cached)
    return cached;

  // Search a file from the include paths.
  for(int i = 0; i < strarray_size(&cpp_include_paths); i++) {
    char* path = cpp_format("%s/%s", strarray_AT(&cpp_include_paths, i), filename);

    if(!file_exists(path))
      continue;

    hashmap_put(&cache, filename, path);
    cpp_include_next_idx = i + 1;
    return path;
  }

  return NULL;
}
