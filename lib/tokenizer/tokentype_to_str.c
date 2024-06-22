#include "../tokenizer_internal.h"

const char*
tokentype_to_str(enum tokentype tt) {
  switch((unsigned)tt) {
    case TT_IDENTIFIER: return "iden";
    case TT_WIDECHAR_LIT: return "widechar";
    case TT_WIDESTRING_LIT: return "widestring";
    case TT_SQSTRING_LIT: return "single-quoted string";
    case TT_DQSTRING_LIT: return "double-quoted string";
    case TT_ELLIPSIS: return "ellipsis";
    case TT_HEX_INT_LIT: return "hexint";
    case TT_OCT_INT_LIT: return "octint";
    case TT_DEC_INT_LIT: return "decint";
    case TT_FLOAT_LIT: return "float";
    case TT_SEP: return "separator";
    case TT_UNKNOWN: return "unknown";
    case TT_OVERFLOW: return "overflow";
    case TT_EOF: return "eof";
  }

  if((int)tt >= TT_CUSTOM)
    return "custom";

  return "????";
}
