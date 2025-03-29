#include "../cpp.h"
#include "../cpp_internal.h"
#include "../strarray.h"

char*
cpp_search_include_next(char* filename) {
  for(; cpp_include_next_idx < strarray_size(&cpp_include_paths); cpp_include_next_idx++) {
    char* path = cpp_format("%s/%s", strarray_AT(&cpp_include_paths, cpp_include_next_idx), filename);

    if(file_exists(path))
      return path;
  }

  return NULL;
}
