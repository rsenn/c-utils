#include "../cpp_internal.h"
#include "../byte.h"
#include "../alloc.h"

cpp_token*
cpp_tokenize_file(char* path) {
  char* p;

  if(!(p = cpp_read_file(path)))
    return NULL;

  /* UTF-8 texts may start with a 3-byte "BOM" marker sequence.
     If exists, just cpp_skip them because they are useless bytes.
     (It is actually not recommended to add BOM markers to UTF-8
     texts, but it's not uncommon particularly on Windows.) */
  /*if(byte_equal(p, 3, "\xef\xbb\xbf"))
    p += 3;*/

  cpp_canonicalize_newline(p);
  cpp_remove_backslash_newline(p);
  cpp_convert_universal_chars(p);

  /* Save the filename for assembler .file directive. */
  cpp_file* file = cpp_file_new(path, (cpp_ctx_get()->in_file_no) + 1, p);

  /* Save the filename for assembler .file directive. */
  alloc_re(&(cpp_ctx_get()->in_files), 0, sizeof(char*) * ((cpp_ctx_get()->in_file_no) + 2));
  (cpp_ctx_get()->in_files)[(cpp_ctx_get()->in_file_no)] = file;
  (cpp_ctx_get()->in_files)[(cpp_ctx_get()->in_file_no) + 1] = NULL;
  (cpp_ctx_get()->in_file_no)++;

  /*(cpp_ctx_get()->cur_file) = file;*/
  return cpp_tokenize(file);
}
