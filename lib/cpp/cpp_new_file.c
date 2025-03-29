#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h" 

cpp_file *cpp_new_file(char *name, int file_no, char *contents) {
  cpp_file *file = alloc_zero(sizeof(cpp_file));
  file->name = name;
  file->display_name = name;
  file->file_no = file_no;
  file->contents = contents;
  return file;
}
