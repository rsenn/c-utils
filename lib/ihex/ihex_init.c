#include "../ihex.h"
#include "../byte.h"
#include "../list.h"

void
ihex_init(ihex_file* ihf) {
  byte_zero(ihf, sizeof(ihex_file));
  init_list_head(&ihf->records);
}
