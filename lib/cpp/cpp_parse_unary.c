#include "../cpp_internal.h"

int
cpp_parse_unary(tokenizer* t, token* tok, int* err) {
  int ret;

  switch((unsigned)tok->type) {
    case TT_IDENTIFIER: ret = 0; break;
    case TT_WIDECHAR_LIT:
    case TT_SQSTRING_LIT: return charlit_to_int(t->buf);
    case TT_NEG: return ~cpp_parse_expr(t, cpp_bp(tok->type), err);
    case TT_PLUS: return cpp_parse_expr(t, cpp_bp(tok->type), err);
    case TT_MINUS: return -cpp_parse_expr(t, cpp_bp(tok->type), err);
    case TT_LNOT: return !cpp_parse_expr(t, cpp_bp(tok->type), err);

    case TT_LPAREN: {
      int inner = cpp_parse_expr(t, 0, err);
      const char* values[] = {")", 0};

      if(0 != cpp_parse_expect(t, TT_RPAREN, values, tok)) {
        cpp_msg_error("missing ')'", t, tok);
        return 0;
      }

      return inner;
    }

    case TT_FLOAT_LIT: {
      cpp_msg_error("floating constant in preprocessor expression", t, tok);
      *err = 1;
      return 0;
    }

    case TT_HEX_INT_LIT: scan_xint(t->buf, (unsigned int*)&ret); break;
    case TT_OCT_INT_LIT: scan_8int(t->buf, (unsigned int*)&ret); break;
    case TT_DEC_INT_LIT: scan_int(t->buf, &ret); break;

    case TT_RPAREN:
    default: {
      cpp_msg_error("unexpected token", t, tok);
      *err = 1;
      return 0;
    }
  }

  return ret;
}
