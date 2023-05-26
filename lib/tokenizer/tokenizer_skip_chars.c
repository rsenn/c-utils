#include "../tokenizer_internal.h"

int
tokenizer_skip_chars(tokenizer* t, const char* chars, int* count) {
  int c;

  assert(!t->peeking);
  *count = 0;

  while(1) {
    const char* s = chars;
    int match = 0;

    if((c = tokenizer_getc(t)) == TOKENIZER_EOF)
      return 0;

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
