#include "../cpp.h"
#include "../cpp_internal.h"

bool
cpp_is_hash(cpp_token* tok) {
  return tok->at_bol && cpp_equal(tok, "#");
}
