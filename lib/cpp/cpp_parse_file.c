#include "../cpp_internal.h"

int
cpp_parse_file(cpp_t* cpp, buffer* f, const char* fn, buffer* out) {
  tokenizer t;
  struct token_s curr;
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

  while((ret = tokenizer_next(&t, &curr)) && curr.type != TT_EOF) {
    newline = curr.column == 0;
    if(newline) {
      ret = eat_whitespace(&t, &curr, &ws_count);
      if(!ret)
        return ret;
    }
    if(curr.type == TT_EOF)
      break;
    if(skip_conditional_block && !(newline && is_char(&curr, '#')))
      continue;
    if(is_char(&curr, '#')) {
      int index;
      if(!newline) {
        error("stray #", &t, &curr);
        return 0;
      }
      if((index = expect(&t, TT_IDENTIFIER, directives, &curr)) == -1) {
        if(skip_conditional_block)
          continue;
        error("invalid preprocessing directive", &t, &curr);
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
        case 0:
          ret = cpp_include_file(cpp, &t, out);
          if(!ret)
            return ret;
          break;
        case 1:
          ret = emit_error_or_warning(&t, 1);
          if(!ret)
            return ret;
          break;
        case 2:
          ret = emit_error_or_warning(&t, 0);
          if(!ret)
            return ret;
          break;
        case 3:
          ret = cpp_parse_macro(cpp, &t);
          if(!ret)
            return ret;
          break;
        case 4:
          if(!skip_next_and_ws(&t, &curr))
            return 0;
          if(curr.type != TT_IDENTIFIER) {
            error("expected identifier", &t, &curr);
            return 0;
          }
          cpp_undef_macro(cpp, t.buf);
          break;
        case 5: // if
          if(all_levels_active()) {
            char* visited[MAX_RECURSION] = {0};
            if(!cpp_evaluate_condition(cpp, &t, &ret, visited))
              return 0;
            free_visited(visited);
            set_level(if_level + 1, ret);
          } else {
            set_level(if_level + 1, 0);
          }
          break;
        case 6: // elif
          if(prev_level_active() && if_level_satisfied < if_level) {
            char* visited[MAX_RECURSION] = {0};
            if(!cpp_evaluate_condition(cpp, &t, &ret, visited))
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
        case 7: // else
          if(prev_level_active() && if_level_satisfied < if_level) {
            if(1) {
              if_level_active = if_level;
              if_level_satisfied = if_level;
            }
          } else if(if_level_active == if_level) {
            --if_level_active;
          }
          break;
        case 8: // ifdef
        case 9: // ifndef
          if(!skip_next_and_ws(&t, &curr) || curr.type == TT_EOF)
            return 0;
          ret = !!cpp_get_macro(cpp, t.buf);
          if(index == 9)
            ret = !ret;

          if(all_levels_active()) {
            set_level(if_level + 1, ret);
          } else {
            set_level(if_level + 1, 0);
          }
          break;
        case 10: // endif
          set_level(if_level - 1, -1);
          break;
        case 11: // line
          ret = tokenizer_read_until(&t, "\n", 1);
          if(!ret) {
            error("unknown", &t, &curr);
            return 0;
          }
          break;
        case 12: // pragma
          buffer_puts(out, "#pragma");
          while((ret = x_tokenizer_next(&t, &curr)) && curr.type != TT_EOF) {
            emit_token(out, &curr, t.buf);
            if(is_char(&curr, '\n'))
              break;
          }
          if(!ret)
            return ret;
          break;
        default: break;
      }
      continue;
    } else {
      while(ws_count) {
        buffer_puts(out, " ");
        --ws_count;
      }
    }
#if DEBUG_CPP
    buffer_putm_internal(buffer_2, "(", fn ? fn : "stdin", ":", 0);
    buffer_putulong(buffer_2, curr.line);
    buffer_putc(buffer_2, ',');
    buffer_putulong(buffer_2, curr.column);
    buffer_puts(buffer_2, ") ");
    if(curr.type == TT_SEP) {
      unsigned char sep = curr.value == '\n' ? ' ' : curr.value;
      buffer_puts(buffer_2, "separator: '");
      buffer_putc(buffer_2, sep);
      buffer_puts(buffer_2, "' 0x");
      buffer_putxlong(buffer_2, sep);

    } else {
      buffer_putm_internal(buffer_2, tokentype_to_str(curr.type), ": ", t.buf, 0);
    }
    buffer_putnlflush(buffer_2);
#endif
    if(curr.type == TT_IDENTIFIER) {
      char* visited[MAX_RECURSION] = {0};
      if(!cpp_expand_macro(cpp, &t, out, t.buf, 0, visited))
        return 0;
      free_visited(visited);
    } else {
      emit_token(out, &curr, t.buf);
    }
  }
  if(if_level) {
    error("unterminated #if", &t, &curr);
    return 0;
  }
  return 1;
}
