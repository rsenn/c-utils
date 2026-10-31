#include "../cpp_internal.h"

/* Releases resources owned by *pp without freeing pp itself, since
   cpp_init() never allocated it. */
void
cpp_free(cpp_ctx* pp) {
  strarray_free(&pp->inc_array);
  strarray_free(&pp->inc_paths);
  strarray_free(&pp->inc_missing);
  strarray_free(&pp->inc_system);
  strarray_free(&pp->undef_tested);
  strarray_free(&pp->undef_valued);
  strarray_free(&pp->undef_cmp);
  strarray_free(&pp->defined_names);
}
