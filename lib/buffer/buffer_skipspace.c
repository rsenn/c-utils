#include <ctype.h>
#include "../buffer.h"

int
buffer_skipspace(buffer* b) {
  return buffer_skip_pred(b, isspace);
}
