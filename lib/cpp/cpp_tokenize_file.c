#include "../cpp_internal.h"
#include "../byte.h"
#include "../alloc.h"

cpp_file **cpp_input_files = 0, *cpp_current_file = 0;

cpp_token*
cpp_tokenize_file(char* path) {
  char* p;

  if(!(p = cpp_read_file(path)))
    return NULL;

  /* UTF-8 texts may start with a 3-byte "BOM" marker sequence.
     If exists, just cpp_skip them because they are useless bytes.
     (It is actually not recommended to add BOM markers to UTF-8
     texts, but it's not uncommon particularly on Windows.) */
  if(byte_equal(p, 3, "\xef\xbb\xbf"))
    p += 3;

  cpp_canonicalize_newline(p);
  cpp_remove_backslash_newline(p);
  cpp_convert_universal_chars(p);

  /* Save the filename for assembler .file directive. */
  static int file_no;
  cpp_file* file = cpp_new_file(path, file_no + 1, p);

  /* Save the filename for assembler .file directive. */
  alloc_re(&cpp_input_files, 0, sizeof(char*) * (file_no + 2));
  cpp_input_files[file_no] = file;
  cpp_input_files[file_no + 1] = NULL;
  file_no++;

  return cpp_tokenize(file);
}
