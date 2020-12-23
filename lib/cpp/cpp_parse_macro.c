#include "../cpp_internal.h"

 int
cpp_parse_macro(struct cpp* cpp, struct tokenizer_s* t) {
  int ws_count;
  int ret = tokenizer_skip_chars(t, " \t", &ws_count);
  if(!ret)
    return ret;
  struct token curr; // tmp = {.column = t->column, .line = t->line};
  ret = tokenizer_next(t, &curr) && curr.type != TT_EOF;
  if(!ret) {
    error("parsing macro name", t, &curr);
    return ret;
  }
  if(curr.type != TT_IDENTIFIER) {
    error("expected identifier", t, &curr);
    return 0;
  }
  const char* macroname = str_dup(t->buf);
#ifdef DEBUG
  dprintf(2, "parsing macro %s\n", macroname);
#endif
  int redefined = 0;
  if(cpp_get_macro(cpp, macroname)) {
    if(!strcmp(macroname, "defined")) {
      error("\"defined\" cannot be used as a macro name", t, &curr);
      return 0;
    }
    redefined = 1;
  }

  struct macro new = {0};
  unsigned macro_flags = MACRO_FLAG_OBJECTLIKE;
  LIST_NEW(new.argnames);

  ret = x_tokenizer_next(t, &curr) && curr.type != TT_EOF;
  if(!ret)
    return ret;

  if(is_char(&curr, '(')) {
    macro_flags = 0;
    unsigned expected = 0;
    while(1) {
      /* process next function argument identifier */
      ret = consume_nl_and_ws(t, &curr, expected);
      if(!ret) {
        error("unexpected", t, &curr);
        return ret;
      }
      expected = 0;
      if(curr.type == TT_SEP) {
        switch(curr.value) {
          case '\\': expected = '\n'; continue;
          case ',': continue;
          case ')':
            ret = tokenizer_skip_chars(t, " \t", &ws_count);
            if(!ret)
              return ret;
            goto break_loop1;
          default: error("unexpected character", t, &curr); return 0;
        }
      } else if(!(curr.type == TT_IDENTIFIER || curr.type == TT_ELLIPSIS)) {
        error("expected identifier for macro arg", t, &curr);
        return 0;
      }
      {
        if(curr.type == TT_ELLIPSIS) {
          if(macro_flags & MACRO_FLAG_VARIADIC) {
            error("\"...\" isn't the last parameter", t, &curr);
            return 0;
          }
          macro_flags |= MACRO_FLAG_VARIADIC;
        }
        char* tmps = str_dup(t->buf);
        LIST_PUSH_BACK(new.argnames, tmps);
      }
      ++new.num_args;
    }
  break_loop1:;
  } else if(is_whitespace_token(&curr)) {
    ret = tokenizer_skip_chars(t, " \t", &ws_count);
    if(!ret)
      return ret;
  } else if(is_char(&curr, '\n')) {
    /* content-less macro */
    goto done;
  }

  struct FILE_container {
    buffer* f;
    char* buf;
    size_t len;
  } contents;
  contents.f = memstream_open(&contents.buf, &contents.len);

  int backslash_seen = 0;
  while(1) {
    /* ignore unknown tokens in macro body */
    ret = tokenizer_next(t, &curr);
    if(!ret)
      return 0;
    if(curr.type == TT_EOF)
      break;
    if(curr.type == TT_SEP) {
      if(curr.value == '\\')
        backslash_seen = 1;
      else {
        if(curr.value == '\n' && !backslash_seen)
          break;
        emit_token(contents.f, &curr, t->buf);
        backslash_seen = 0;
      }
    } else {
      emit_token(contents.f, &curr, t->buf);
    }
  }
  new.str_contents = buffer_reopen(contents.f, &contents.buf, &contents.len);
  new.str_contents_buf = contents.buf;
done:
  if(redefined) {
    struct macro* old = cpp_get_macro(cpp, macroname);
    char* s_old = old->str_contents_buf ? old->str_contents_buf : "";
    char* s_new = new.str_contents_buf ? new.str_contents_buf : "";
    if(strcmp(s_old, s_new)) {
      char buf[128];
      size_t n;
      n = str_copy(buf, "redefinition of macro ");
      str_copyn(&buf[n], macroname, sizeof(buf) - n);

      warning(buf, t, 0);
    }
  }
  new.num_args |= macro_flags;
  cpp_add_macro(cpp, macroname, &new);
  return 1;
}
