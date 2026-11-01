#include "../json.h"

void
ndjson_free(ndjson* nd) {
  stralloc_free(&nd->line);
}
