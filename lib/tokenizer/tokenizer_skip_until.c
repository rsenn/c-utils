#include "../tokenizer_internal.h"

void
tokenizer_skip_until(tokenizer* t, const char* marker) {
  ignore_until(t, marker, 0);
}
