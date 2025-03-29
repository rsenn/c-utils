#include "../cpp.h"
#include "../cpp_internal.h"

/* This function is used for the stringizing operator (#). */
cpp_token*
cpp_stringize(cpp_token* hash, cpp_token* arg) {
  /* Create a new string token. We need to set some value to its
     source location for error reporting function, so we use a macro
     name token as a template. */
  char* s = cpp_join_tokens(arg, NULL);

  return cpp_new_str_token(s, hash);
}
