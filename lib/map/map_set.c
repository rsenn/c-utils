#include "../byte.h"
#include "../map_internal.h"

int
map_set_(map_base_t* m, const char* key, void* value, int vsize) {
  int n, err;
  map_node_t **next, *node;
  /* Find & replace existing node */
  next = map_getref(m, key);
  if(next) {
    byte_copy((*next)->value, vsize, value);
    return 0;
  }
  /* Add new node */
  node = map_newnode(key, value, vsize);
  if(node == NULL) goto fail;
  if(m->nnodes >= m->nbuckets) {
    n = (m->nbuckets > 0) ? (m->nbuckets << 1) : 1;
    err = map_resize(m, n);
    if(err) goto fail;
  }
  map_addnode(m, node);
  m->nnodes++;
  return 0;
fail:
  if(node) free(node);
  return -1;
}
