#include "../tokenizer_internal.h"

void
tokenizer_init(tokenizer* t, buffer* in, int flags) {
  *t = (tokenizer){.input = in, .line = 1, .flags = flags, .bufsize = MAX_TOK_LEN};
}
