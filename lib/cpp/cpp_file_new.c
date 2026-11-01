#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

cpp_file*
cpp_file_new(const char* name, int file_no, char* contents) {
  cpp_file* file;

  if((file = alloc_zero(sizeof(cpp_file)))) {
    file->name = str_dup(name);
    file->display_name = str_dup(name);
    file->file_no = file_no;
    file->contents = contents;
    file->ref_count = 1;
  }

  return file;
}
