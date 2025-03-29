#include "../cpp.h"
#include "../cpp_internal.h"
#include <string.h>
#include <errno.h>

hashmap cpp_pragma_once = HASHMAP_INIT();

cpp_token*
cpp_include_file(cpp_token* tok, char* path, cpp_token* filename_tok) {
  static hashmap include_guards;
  cpp_token* tok2;
  char* guard_name;

  /* Check for "#pragma once" */
  if(hashmap_get(&cpp_pragma_once, path))
    return tok;

  /* If we read the same file before, and if the file was guarded
     by the usual #ifndef ... #endif pattern, we may be able to
     cpp_skip the file without opening it. */
  if((guard_name = hashmap_get(&include_guards, path)) && hashmap_get(&cpp_macros, guard_name))
    return tok;

  if(!(tok2 = cpp_tokenize_file(path)))
    cpp_error_tok(filename_tok, "%s: cannot open file: %s", path, strerror(errno));

  if((guard_name = cpp_detect_include_guard(tok2)))
    hashmap_put(&include_guards, path, guard_name);

  return cpp_append(tok2, tok);
}
