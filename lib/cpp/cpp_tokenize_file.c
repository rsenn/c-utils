#include "../cpp_internal.h"
#include "../byte.h"
#include "../alloc.h"
#include "../str.h"

cpp_token*
cpp_tokenize_file(cpp_ctx* pp, char* path) {
  cpp_file* file;
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

  /* dependency scan: the text of #error and #warning is prose, whose
     apostrophes would not tokenize */
  if(pp->scan_only) {
    char* q = p;

    while(*q) {
      char* e = q;

      while(*e == ' ' || *e == '\t')
        ++e;

      if(*e == '#') {
        ++e;

        while(*e == ' ' || *e == '\t')
          ++e;

        if(!str_diffn(e, "error", 5) || !str_diffn(e, "warning", 7)) {
          for(e += *e == 'e' ? 5 : 7; *e && *e != '\n'; ++e)
            *e = ' ';
        }
      }

      while(*q && *q != '\n')
        ++q;

      if(*q)
        ++q;
    }
  }

  cpp_remove_backslash_newline(p);
  cpp_convert_universal_chars(p);

  /* Save the filename for assembler .file directive. */
  file = cpp_file_new(path, pp->in_file_no + 1, p);

  /* Save the filename for assembler .file directive. */
  alloc_re(&pp->in_files, 0, sizeof(char*) * (pp->in_file_no + 2));
  pp->in_files[pp->in_file_no] = file;
  pp->in_files[pp->in_file_no + 1] = NULL;
  pp->in_file_no++;

  /*pp->cur_file = file;*/
  return cpp_tokenize(pp, file);
}
