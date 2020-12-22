#include "../tokenizer_internal.h"

int
tokenizer_read_until(tokenizer* t, const char* marker, int stop_at_nl) {
  int c, marker_is_nl = !strcmp(marker, "\n");
  char* s = t->buf;
  while(1) {
    c = tokenizer_getc(t);
    if(c == EOF) {
      *s = 0;
      return 0;
    }
    if(c == '\n') {
      t->line++;
      t->column = 0;
      if(stop_at_nl) {
        *s = 0;
        if(marker_is_nl)
          return 1;
        return 0;
      }
    }
    if(!sequence_follows(t, c, marker))
      s = assign_bufchar(t, s, c);
    else
      break;
  }
  *s = 0;
  size_t i;
  for(i = str_len(marker); i > 0;) tokenizer_ungetc(t, marker[--i]);
  return 1;
}
