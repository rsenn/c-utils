#include "../cpp_internal.h"

unsigned
cpp_get_macro_info(cpp_t* cpp, tokenizer* t, struct macro_info_s* mi_list, size_t* mi_cnt, unsigned nest, unsigned tpos, const char* name, char* visited[], unsigned rec_level) {
  int brace_lvl = 0;
  while(1) {
    struct token_s tok;
    int ret = tokenizer_next(t, &tok);
    if(!ret || tok.type == TT_EOF)
      break;
#ifdef DEBUG_CPP
    buffer_putm_internal(buffer_2, "(", name, ") nest ", 0);

    buffer_putulong(buffer_2, nest);
    buffer_puts(buffer_2, ", brace ");
    buffer_putulong(buffer_2, brace_lvl);
    buffer_puts(buffer_2, " t: ");
    buffer_puts(buffer_2, t->buf);
    buffer_putnlflush(buffer_2);
#endif
    struct macro_s* m = 0;
    if(tok.type == TT_IDENTIFIER && (m = cpp_get_macro(cpp, t->buf)) && !was_visited(t->buf, visited, rec_level)) {
      const char* newname = str_dup(t->buf);
      if(FUNCTIONLIKE(m)) {
        if(tokenizer_peek(t) == '(') {
          unsigned tpos_save = tpos;
          tpos = cpp_get_macro_info(cpp, t, mi_list, mi_cnt, nest + 1, tpos + 1, newname, visited, rec_level);
          mi_list[*mi_cnt] = (struct macro_info_s){.name = newname, .nest = nest + 1, .first = tpos_save, .last = tpos + 1};
          ++(*mi_cnt);
        } else {
          /* suppress expansion */
        }
      } else {
        mi_list[*mi_cnt] = (struct macro_info_s){.name = newname, .nest = nest + 1, .first = tpos, .last = tpos + 1};
        ++(*mi_cnt);
      }
    } else if(is_char(&tok, '(')) {
      ++brace_lvl;
    } else if(is_char(&tok, ')')) {
      --brace_lvl;
      if(brace_lvl == 0 && nest != 0)
        break;
    }
    ++tpos;
  }
  return tpos;
}
