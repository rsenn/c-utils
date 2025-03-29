#include "../cpp.h"
#include "../cpp_internal.h"
#include "../str.h"
#include <assert.h>

cpp_string_kind
cpp_get_string_kind(cpp_token* tok) {
  if(str_equal(tok->loc, "u8"))
    return STR_UTF8;

  switch(tok->loc[0]) {
    case '"': return STR_NONE;
    case 'u': return STR_UTF16;
    case 'U': return STR_UTF32;
    case 'L': return STR_WIDE;
  }

  assert(0);
}
