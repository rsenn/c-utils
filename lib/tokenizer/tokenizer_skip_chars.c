#include "../tokenizer_internal.h"

int
tokenizer_skip_chars(tokenizer* t, const char* chars, int* count) {
  assert(!t->peeking);
  int c;
  *count = 0;
  while(1) {
    c = tokenizer_getc(t);
    if(c == TOKENIZER_EOF)
      return 0;
    const char* s = chars;
    int match = 0;
    while(*s) {
      if(c == *s) {
        ++(*count);
        match = 1;
        break;
      }
      ++s;
    }
    if(!match) {
      tokenizer_ungetc(t, c);
      return 1;
    }
  }
}
