#include "../cpp_internal.h"

 int
cpp_evaluate_condition(struct cpp* cpp, struct tokenizer_s* t, int* result, char* visited[]) {
  int ret, backslash_seen = 0;
  struct token curr;
  char* bufp;
  size_t size;
  int tflags = tokenizer_get_flags(t);
  tokenizer_set_flags(t, tflags | TF_PARSE_WIDE_STRINGS);
  ret = tokenizer_next(t, &curr);
  if(!ret)
    return ret;
  if(!is_whitespace_token(&curr)) {
    error("expected whitespace after if/elif", t, &curr);
    return 0;
  }
  buffer* f = memstream_open(&bufp, &size);
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
#ifdef DEBUG
  dprintf(2, "evaluating condition %s\n", bufp);
#endif
  struct tokenizer_s t2;
  tokenizer_from_file(&t2, f);
  ret = do_eval(&t2, result);
  buffer_close(f);
  free(bufp);
  tokenizer_set_flags(t, tflags);
  return ret;
}
