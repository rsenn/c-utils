#include "../cpp_internal.h"

int
cpp_parse_file(cpp* pp, buffer* f, const char* fn, buffer* out) {
  tokenizer t;
  token tok;
  int ret, newline = 1, ws_count = 0, if_level = 0, if_level_active = 0, if_level_satisfied = 0;
  static const char* directives[] = {
      "include",
      "error",
      "warning",
      "define",
      "undef",
      "if",
      "elif",
      "else",
      "ifdef",
      "ifndef",
      "endif",
      "line",
      "pragma",
      0,
  };

  tokenizer_init(&t, f, TF_PARSE_STRINGS);
  tokenizer_set_filename(&t, fn);
  tokenizer_register_marker(&t, MT_MULTILINE_COMMENT_START, "/*"); /**/
  tokenizer_register_marker(&t, MT_MULTILINE_COMMENT_END, "*/");
  tokenizer_register_marker(&t, MT_SINGLELINE_COMMENT_START, "//");

#define all_levels_active() (if_level_active == if_level)
#define prev_level_active() (if_level_active == if_level - 1)
#define set_level(X, V) \
  do { \
    if(if_level_active > X) \
      if_level_active = X; \
    if(if_level_satisfied > X) \
      if_level_satisfied = X; \
    if(V != -1) { \
      if(V) \
        if_level_active = X; \
      else if(if_level_active == X) \
        if_level_active = X - 1; \
      if(V && if_level_active == X) \
        if_level_satisfied = X; \
    } \
    if_level = X; \
  } while(0)
#define skip_conditional_block (if_level > if_level_active)

  while((ret = tokenizer_next(&t, &tok)) && tok.type != TT_EOF) {
    if((newline = tok.column == 0))
      if(!cpp_parse_whitespace(&t, &tok, &ws_count))
        return 0;

    if(tok.type == TT_EOF)
      break;

    if(skip_conditional_block && !(newline && token_is_char(&tok, '#')))
      continue;

    if(token_is_char(&tok, '#')) {
      int index;

      if(!newline) {
        error("stray #", &t, &tok);
        return 0;
      }

      if((index = cpp_parse_expect(&t, TT_IDENTIFIER, directives, &tok)) == -1) {
        if(skip_conditional_block)
          continue;

        error("invalid preprocessing directive", &t, &tok);
        return 0;
      }

      if(skip_conditional_block)

        switch(index) {
          case 0:
          case 1:
          case 2:
          case 3:
          case 4:
          case 11:
          case 12: continue;
          default: break;
        }

      switch(index) {
        case 0: {
          if(!cpp_include_file(pp, &t, out))
            return 0;

          break;
        }

        case 1: {
          if(!cpp_parse_error(&t, 1))
            return 0;

          break;
        }

        case 2: {
          if(!cpp_parse_error(&t, 0))
            return 0;

          break;
        }

        case 3: {
          if(!cpp_macro_parse(pp, &t))
            return 0;

          break;
        }

        case 4: {
          if(!cpp_parse_skip(&t, &tok))
            return 0;

          if(tok.type != TT_IDENTIFIER) {
            error("expected identifier", &t, &tok);
            return 0;
          }

          cpp_macro_undef(pp, t.buf);
          break;
        }

        case 5: { // if
          if(all_levels_active()) {
            char* visited[MAX_RECURSION] = {0};

            if(!cpp_evaluate_condition(pp, &t, &ret, visited))
              return 0;

            free_visited(visited);
            set_level(if_level + 1, ret);
          } else {
            set_level(if_level + 1, 0);
          }

          break;
        }

        case 6: { // elif
          if(prev_level_active() && if_level_satisfied < if_level) {
            char* visited[MAX_RECURSION] = {0};

            if(!cpp_evaluate_condition(pp, &t, &ret, visited))
              return 0;

            free_visited(visited);

            if(ret) {
              if_level_active = if_level;

              if_level_satisfied = if_level;
            }
          } else if(if_level_active == if_level) {
            --if_level_active;
          }

          break;
        }

        case 7: { // else
          if(prev_level_active() && if_level_satisfied < if_level) {
            if(1) {
              if_level_active = if_level;

              if_level_satisfied = if_level;
            }
          } else if(if_level_active == if_level) {
            --if_level_active;
          }

          break;
        }

        case 8:   // ifdef
        case 9: { // ifndef
          if(!cpp_parse_skip(&t, &tok) || tok.type == TT_EOF)
            return 0;

          ret = !!cpp_macro_get(pp, t.buf);

          if(index == 9)
            ret = !ret;

          set_level(if_level + 1, all_levels_active() ? ret : 0);
          break;
        }

        case 10: { // endif
          set_level(if_level - 1, -1);
          break;
        }

        case 11: { // line
          if(!(ret = tokenizer_read_until(&t, "\n", 1))) {
            error("unknown", &t, &tok);
            return 0;
          }

          break;
        }

        case 12: { // pragma
          buffer_puts(out, "#pragma");

          while((ret = x_tokenizer_next(&t, &tok)) && tok.type != TT_EOF) {
            emit_token(out, &tok, t.buf);

            if(token_is_char(&tok, '\n'))
              break;
          }

          if(!ret)
            return ret;

          break;
        }

        default: {
          break;
        }
      }

      continue;
    } else {
      while(ws_count) {
        buffer_puts(out, " ");
        --ws_count;
      }
    }

#if DEBUG_CPP_TOKEN
    buffer_putm_internal(buffer_2, "token (", fn ? fn : "stdin", ":", NULL);
    buffer_putulong(buffer_2, tok.line);
    buffer_putc(buffer_2, ':');
    buffer_putulong(buffer_2, tok.column);
    buffer_puts(buffer_2, ") ");

    if(tok.type == TT_SEP) {
      unsigned char sep = tok.value == '\n' ? ' ' : tok.value;

      buffer_puts(buffer_2, "separator: '");
      buffer_putc(buffer_2, sep);
      buffer_puts(buffer_2, "' 0x");
      buffer_putxlong(buffer_2, sep);

    } else {
      buffer_putm_internal(buffer_2, tokentype_to_str(tok.type), ": ", t.buf, NULL);
    }

    buffer_putnlflush(buffer_2);
#endif

    if(tok.type == TT_IDENTIFIER) {
      char* visited[MAX_RECURSION] = {0};

      if(!cpp_macro_expand(pp, &t, out, t.buf, 0, visited))
        return 0;

      free_visited(visited);
    } else {
      emit_token(out, &tok, t.buf);
    }
  }

  if(if_level) {
    error("unterminated #if", &t, &tok);
    return 0;
  }

  return 1;
}
