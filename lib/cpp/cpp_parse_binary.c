#include "../cpp_internal.h"

int
cpp_parse_binary(tokenizer* t, int left, token* tok, int* err) {
  int right;

  switch((unsigned)tok->type) {
    case TT_LAND:
    case TT_LOR: {
      right = cpp_parse_expr(t, cpp_bp(tok->type), err);

      if(tok->type == TT_LAND)
        return left && right;

      return left || right;
    }

    case TT_LTE: return left <= cpp_parse_expr(t, cpp_bp(tok->type), err);
    case TT_GTE: return left >= cpp_parse_expr(t, cpp_bp(tok->type), err);
    case TT_SHL: return left << cpp_parse_expr(t, cpp_bp(tok->type), err);
    case TT_SHR: return left >> cpp_parse_expr(t, cpp_bp(tok->type), err);
    case TT_EQ: return left == cpp_parse_expr(t, cpp_bp(tok->type), err);
    case TT_NEQ: return left != cpp_parse_expr(t, cpp_bp(tok->type), err);
    case TT_LT: return left < cpp_parse_expr(t, cpp_bp(tok->type), err);
    case TT_GT: return left > cpp_parse_expr(t, cpp_bp(tok->type), err);
    case TT_BAND: return left & cpp_parse_expr(t, cpp_bp(tok->type), err);
    case TT_BOR: return left | cpp_parse_expr(t, cpp_bp(tok->type), err);
    case TT_XOR: return left ^ cpp_parse_expr(t, cpp_bp(tok->type), err);
    case TT_PLUS: return left + cpp_parse_expr(t, cpp_bp(tok->type), err);
    case TT_MINUS: return left - cpp_parse_expr(t, cpp_bp(tok->type), err);
    case TT_MUL: return left * cpp_parse_expr(t, cpp_bp(tok->type), err);

    case TT_DIV:
    case TT_MOD: {
      if((right = cpp_parse_expr(t, cpp_bp(tok->type), err)) == 0) {
        cpp_msg_error("eval: div by zero", t, tok);
        *err = 1;
      } else if(tok->type == TT_DIV) {
        return left / right;
      } else if(tok->type == TT_MOD) {
        return left % right;
      }

      return 0;
    }

    default: {
      cpp_msg_error("eval: unexpect token", t, tok);
      *err = 1;
      return 0;
    }
  }
}
