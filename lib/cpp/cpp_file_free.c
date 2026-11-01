#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

void
cpp_file_free(cpp_file* file) {
  if(--file->ref_count == 0) {
    if(file->name)
      alloc_free(file->name);

    if(file->display_name)
      alloc_free(file->display_name);

    if(file->contents)
      alloc_free(file->contents);

    alloc_free(file);
  }
}
