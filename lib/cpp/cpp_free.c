#include "../cpp_internal.h"

/* Releases resources owned by *pp without freeing pp itself, since
   cpp_init() never allocated it. */
void
cpp_free(cpp_ctx* pp) {
  strarray_free(&pp->inc_array);
  strarray_free(&pp->inc_paths);
}
