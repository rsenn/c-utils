#include "../cpp_internal.h"

unsigned
cpp_macro_get_info(cpp* pp, tokenizer* t, cpp_macro_info* mi_list, size_t* mi_cnt, unsigned nest, unsigned tpos, const char* name, char* visited[], unsigned rec_level) {
  int brace_lvl = 0;

  for(;;) {
    token tok;
    cpp_macro* m = 0;
    int ret;

    if(!(ret = tokenizer_next(t, &tok)) || tok.type == TT_EOF)
      break;

#ifdef DEBUG_CPP
    buffer_putm_internal(buffer_2, "(", name, ") nest ", NULL);

    buffer_putulong(buffer_2, nest);
    buffer_puts(buffer_2, ", brace ");
    buffer_putulong(buffer_2, brace_lvl);
    buffer_puts(buffer_2, " t: ");
    buffer_puts(buffer_2, t->buf);
    buffer_putnlflush(buffer_2);
#endif

    if(tok.type == TT_IDENTIFIER && (m = cpp_macro_get(pp, t->buf)) && !was_visited(t->buf, visited, rec_level)) {
      const char* newname = str_dup(t->buf);

      if(FUNCTIONLIKE(m)) {
        if(tokenizer_peek(t) == '(') {
          unsigned tpos_save = tpos;

          tpos = cpp_macro_get_info(pp, t, mi_list, mi_cnt, nest + 1, tpos + 1, newname, visited, rec_level);

          mi_list[*mi_cnt].name = newname;
          mi_list[*mi_cnt].nest = nest + 1;
          mi_list[*mi_cnt].first = tpos_save;
          mi_list[*mi_cnt].last = tpos + 1;

          ++(*mi_cnt);
        } else {
          /* suppress expansion */
        }
      } else {
        mi_list[*mi_cnt].name = newname;
        mi_list[*mi_cnt].nest = nest + 1;
        mi_list[*mi_cnt].first = tpos;
        mi_list[*mi_cnt].last = tpos + 1;

        ++(*mi_cnt);
      }
    } else if(token_is_char(&tok, '(')) {
      ++brace_lvl;
    } else if(token_is_char(&tok, ')')) {
      --brace_lvl;

      if(brace_lvl == 0 && nest != 0)
        break;
    }

    ++tpos;
  }

  return tpos;
}
