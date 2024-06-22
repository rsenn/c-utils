#include "../cpp_internal.h"
#include "../memstream.h"

static int
consume_nl_and_ws(tokenizer* t, token* tok, int expected) {
  if(!x_tokenizer_next(t, tok)) {
  err:
    error("unexpected", t, tok);
    return 0;
  }

  if(expected) {
    if(tok->type != TT_SEP || tok->value != expected)
      goto err;

    switch(expected) {
      case '\\': expected = '\n'; break;
      case '\n': expected = 0; break;
    }
  } else {
    if(is_whitespace_token(tok))
      ;
    else if(is_char(tok, '\\'))
      expected = '\n';
    else
      return 1;
  }

  return consume_nl_and_ws(t, tok, expected);
}

int
cpp_macro_parse(cpp_t* cpp, tokenizer* t) {
  const char* macroname;
  int ws_count, ret, redefined = 0;
  token curr; // tmp = {.column = t->column, .line = t->line};

  if(!(ret = tokenizer_skip_chars(t, " \t", &ws_count)))
    return ret;

  if(!(ret = tokenizer_next(t, &curr) && curr.type != TT_EOF)) {
    error("parsing macro name", t, &curr);
    return ret;
  }

  if(curr.type != TT_IDENTIFIER) {
    error("expected identifier", t, &curr);
    return 0;
  }

  macroname = str_dup(t->buf);

#ifdef DEBUG_CPP
  buffer_puts(buffer_2, "parsing macro ");
  buffer_puts(buffer_2, macroname);
  buffer_putnlflush(buffer_2);
#endif

  if(cpp_macro_get(cpp, macroname)) {
    if(!str_diff(macroname, "defined")) {
      error("\"defined\" cannot be used as a macro name", t, &curr);
      return 0;
    }
    redefined = 1;
  }

  {
    cpp_macro new = {0};
    unsigned macro_flags = MACRO_FLAG_OBJECTLIKE;

    LIST_NEW(new.argnames);

    if(!(ret = x_tokenizer_next(t, &curr) && curr.type != TT_EOF))
      return ret;

    if(is_char(&curr, '(')) {
      unsigned expected = 0;

      macro_flags = 0;

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
          char* tmps;

          if(curr.type == TT_ELLIPSIS) {
            if(macro_flags & MACRO_FLAG_VARIADIC) {
              error("\"...\" isn't the last parameter", t, &curr);
              return 0;
            }

            macro_flags |= MACRO_FLAG_VARIADIC;
          }

          tmps = str_dup(t->buf);
          LIST_PUSH_BACK(new.argnames, tmps);
        }

        ++new.num_args;
      }

    break_loop1:

    } else if(is_whitespace_token(&curr)) {
      ret = tokenizer_skip_chars(t, " \t", &ws_count);

      if(!ret)
        return ret;

    } else if(is_char(&curr, '\n')) {
      /* content-less macro */
      goto done;
    }

    {
      int backslash_seen = 0;
      cpp_file contents = {0, 0, 0};

      contents.f = memstream_open(&contents.buf, &contents.len);

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

      buffer_putc(contents.f, '\0');
      buffer_flush(contents.f);

      if(contents.buf) {
        buffer_copybuf(&new.str_contents, contents.buf, contents.len);

        new.str_contents_buf = new.str_contents.x;
      }
    }

  done:
    if(redefined) {
      cpp_macro* old = cpp_macro_get(cpp, macroname);
      char* s_old = old->str_contents_buf ? old->str_contents_buf : "";
      char* s_new = new.str_contents_buf ? new.str_contents_buf : "";

      if(str_diff(s_old, s_new)) {
        char buf[128];
        size_t n;

        n = str_copy(buf, "redefinition of macro ");
        str_copyn(&buf[n], macroname, sizeof(buf) - n);

        warning(buf, t, 0);
      }
    }

    new.num_args |= macro_flags;
    cpp_macro_add(cpp, macroname, &new);
  }

  return 1;
}
