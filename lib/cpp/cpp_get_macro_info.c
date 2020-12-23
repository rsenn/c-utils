#include "../cpp_internal.h"

unsigned
cpp_get_macro_info(struct cpp* cpp, struct tokenizer_s* t, struct macro_info* mi_list, size_t* mi_cnt, unsigned nest, unsigned tpos, const char* name, char* visited[], unsigned rec_level) {
  int brace_lvl = 0;
  while(1) {
    struct token tok;
    int ret = tokenizer_next(t, &tok);
    if(!ret || tok.type == TT_EOF)
      break;
#ifdef DEBUG
    dprintf(2, "(%s) nest %d, brace %u t: %s\n", name, nest, brace_lvl, t->buf);
#endif
    struct macro* m = 0;
    if(tok.type == TT_IDENTIFIER && (m = cpp_get_macro(cpp, t->buf)) && !was_visited(t->buf, visited, rec_level)) {
      const char* newname = str_dup(t->buf);
      if(FUNCTIONLIKE(m)) {
        if(tokenizer_peek(t) == '(') {
          unsigned tpos_save = tpos;
          tpos = cpp_get_macro_info(cpp, t, mi_list, mi_cnt, nest + 1, tpos + 1, newname, visited, rec_level);
          mi_list[*mi_cnt] = (struct macro_info){.name = newname, .nest = nest + 1, .first = tpos_save, .last = tpos + 1};
          ++(*mi_cnt);
        } else {
          /* suppress expansion */
        }
      } else {
        mi_list[*mi_cnt] = (struct macro_info){.name = newname, .nest = nest + 1, .first = tpos, .last = tpos + 1};
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
