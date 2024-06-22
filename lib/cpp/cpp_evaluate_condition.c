#include "../cpp_internal.h"
#include "../memstream.h"

static int
do_eval(tokenizer* t, int* result) {
  int err = 0;

  tokenizer_register_custom_token(t, TT_LAND, "&&");
  tokenizer_register_custom_token(t, TT_LOR, "||");
  tokenizer_register_custom_token(t, TT_LTE, "<=");
  tokenizer_register_custom_token(t, TT_GTE, ">=");
  tokenizer_register_custom_token(t, TT_SHL, "<<");
  tokenizer_register_custom_token(t, TT_SHR, ">>");
  tokenizer_register_custom_token(t, TT_EQ, "==");
  tokenizer_register_custom_token(t, TT_NEQ, "!=");

  tokenizer_register_custom_token(t, TT_LT, "<");
  tokenizer_register_custom_token(t, TT_GT, ">");

  tokenizer_register_custom_token(t, TT_BAND, "&");
  tokenizer_register_custom_token(t, TT_BOR, "|");
  tokenizer_register_custom_token(t, TT_XOR, "^");
  tokenizer_register_custom_token(t, TT_NEG, "~");

  tokenizer_register_custom_token(t, TT_PLUS, "+");
  tokenizer_register_custom_token(t, TT_MINUS, "-");
  tokenizer_register_custom_token(t, TT_MUL, "*");
  tokenizer_register_custom_token(t, TT_DIV, "/");
  tokenizer_register_custom_token(t, TT_MOD, "%");

  tokenizer_register_custom_token(t, TT_LPAREN, "(");
  tokenizer_register_custom_token(t, TT_RPAREN, ")");
  tokenizer_register_custom_token(t, TT_LNOT, "!");

  *result = expr(t, 0, &err);
#ifdef DEBUG_CPP
  buffer_puts(buffer_2, "eval result: ");
  buffer_putlong(buffer_2, *result);
  buffer_putnlflush(buffer_2);
#endif
  return !err;
}

int
cpp_evaluate_condition(cpp* pp, tokenizer* t, int* result, char* visited[]) {
  int ret, backslash_seen = 0;
  token tok;
  char* x = 0;
  size_t n = 0;
  int flags = tokenizer_get_flags(t);
  buffer* mst;
  tokenizer t2;

  tokenizer_set_flags(t, flags | TF_PARSE_WIDE_STRINGS);

  if(!(ret = tokenizer_next(t, &tok)))
    return ret;

  if(!is_whitespace_token(&tok)) {
    cpp_error("expected whitespace after if/elif", t, &tok);
    return 0;
  }

  mst = memstream_open(&x, &n);

  while(1) {
    if(!(ret = tokenizer_next(t, &tok)))
      return ret;

    if(tok.type == TT_IDENTIFIER) {
      if(!cpp_macro_expand(pp, t, mst, t->buf, -1, visited))
        return 0;

    } else if(tok.type == TT_SEP) {
      if(tok.value == '\\')
        backslash_seen = 1;
      else {
        if(tok.value == '\n') {
          if(!backslash_seen)
            break;
        } else {
          emit_token(mst, &tok, t->buf);
        }

        backslash_seen = 0;
      }
    } else {
      emit_token(mst, &tok, t->buf);
    }
  }

  if(!(mst = memstream_reopen(mst, &x, &n)) /* || n == 0*/) {
    cpp_error("#(el)if with no expression", t, &tok);
    return 0;
  }

#ifdef DEBUG_CPP
  buffer_putm_internal(buffer_2, "evaluating condition ", NULL);
  buffer_put(buffer_2, x, n);
  buffer_putnlflush(buffer_2);
#endif

  tokenizer_from_file(&t2, mst);
  ret = do_eval(&t2, result);
  memstream_free(mst);
  if(x)
    alloc_free(x);

  tokenizer_set_flags(t, flags);
  return ret;
}
