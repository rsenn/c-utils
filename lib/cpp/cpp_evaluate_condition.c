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
cpp_evaluate_condition(cpp_t* cpp, tokenizer* t, int* result, char* visited[]) {
  int ret, backslash_seen = 0;
  struct token_s curr;
  char* bufp;
  size_t size;
  int tflags = tokenizer_get_flags(t);
  buffer* f;
  tokenizer_set_flags(t, tflags | TF_PARSE_WIDE_STRINGS);
  ret = tokenizer_next(t, &curr);
  if(!ret)
    return ret;
  if(!is_whitespace_token(&curr)) {
    error("expected whitespace after if/elif", t, &curr);
    return 0;
  }
  f = memstream_open(&bufp, &size);
  while(1) {
    ret = tokenizer_next(t, &curr);
    if(!ret)
      return ret;
    if(curr.type == TT_IDENTIFIER) {
      if(!cpp_expand_macro(cpp, t, f, t->buf, -1, visited))
        return 0;
    } else if(curr.type == TT_SEP) {
      if(curr.value == '\\')
        backslash_seen = 1;
      else {
        if(curr.value == '\n') {
          if(!backslash_seen)
            break;
        } else {
          emit_token(f, &curr, t->buf);
        }
        backslash_seen = 0;
      }
    } else {
      emit_token(f, &curr, t->buf);
    }
  }
  f = buffer_reopen(f, &bufp, &size);
  if(!f || size == 0) {
    error("#(el)if with no expression", t, &curr);
    return 0;
  }
#ifdef DEBUG_CPP
  buffer_putm_internal(buffer_2, "evaluating condition ", bufp, NULL);
  buffer_putnlflush(buffer_2);
#endif
  {
    tokenizer t2;

    tokenizer_from_file(&t2, f);
    ret = do_eval(&t2, result);
    buffer_close(f);
    alloc_free(bufp);
    tokenizer_set_flags(t, tflags);
    return ret;
  }
}
