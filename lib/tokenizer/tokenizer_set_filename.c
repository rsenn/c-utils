#include "../tokenizer_internal.h"

void
tokenizer_set_filename(tokenizer* t, const char* fn) {
  t->filename = fn;
}
