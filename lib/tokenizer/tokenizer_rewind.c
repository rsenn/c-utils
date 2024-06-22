#include "../tokenizer_internal.h"
#include "../buffer.h"

int
tokenizer_rewind(tokenizer* t) {
  buffer* b = t->input;
  int flags = t->flags;
  const char* fn = t->filename;

  tokenizer_init(t, b, flags);
  tokenizer_set_filename(t, fn);
  buffer_rewind(b);

  return 1;
}
