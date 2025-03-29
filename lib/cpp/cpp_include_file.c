#include "../cpp.h"
#include "../cpp_internal.h"
#include <string.h>
#include <errno.h>

cpp_token *cpp_include_file(cpp_token *tok, char *path, cpp_token *filename_tok) {
  // Check for "#pragma once"
  if (hashmap_get(&cpp_pragma_once, path))
    return tok;

  // If we read the same file before, and if the file was guarded
  // by the usual #ifndef ... #endif pattern, we may be able to
  // skip the file without opening it.
  static hashmap include_guards;
  char *guard_name = hashmap_get(&include_guards, path);
  if (guard_name && hashmap_get(&macros, guard_name))
    return tok;

  cpp_token *tok2 = tokenize_file(path);
  if (!tok2)
    cpp_error_tok(filename_tok, "%s: cannot open file: %s", path, strerror(errno));

  guard_name = cpp_detect_include_guard(tok2);
  if (guard_name)
    hashmap_put(&include_guards, path, guard_name);

  return cpp_append(tok2, tok);
}
