#include "../json.h"
#include "../byte.h"

void
ndjson_init(ndjson* nd) {
  byte_zero(nd, sizeof(ndjson));
}
