#include "../byte.h"
#include "../map_internal.h"

#include <stdlib.h>


static map_node_t*
map_newnode(const char* key, void* value, int vsize) {
  map_node_t* node;
  int ksize = str_len(key) + 1;
  int voffset = ksize + ((sizeof(void*) - ksize) % sizeof(void*));
  node = malloc(sizeof(*node) + voffset + vsize);
  if(!node)
    return NULL;
  byte_copy(node + 1, ksize, key);
  node->hash = map_hash(key);
  node->value = ((char*)(node + 1)) + voffset;
  byte_copy(node->value, vsize, value);
  return node;
}

static void
map_addnode(map_base_t* m, map_node_t* node) {
  int n = map_bucketidx(m, node->hash);
  node->next = m->buckets[n];
  m->buckets[n] = node;
}

static int
map_resize(map_base_t* m, int nbuckets) {
  map_node_t *nodes, *node, *next;
  map_node_t** buckets;
  int i;
  /* Chain all nodes together */
  nodes = NULL;
  i = m->nbuckets;
  while(i--) {
    node = (m->buckets)[i];
    while(node) {
      next = node->next;
      node->next = nodes;
      nodes = node;
      node = next;
    }
  }
  /* Reset buckets */
  buckets = realloc(m->buckets, sizeof(*m->buckets) * nbuckets);
  if(buckets != NULL) {
    m->buckets = buckets;
    m->nbuckets = nbuckets;
  }
  if(m->buckets) {
    byte_zero(m->buckets, sizeof(*m->buckets) * m->nbuckets);
    /* Re-add nodes to buckets */
    node = nodes;
    while(node) {
      next = node->next;
      map_addnode(m, node);
      node = next;
    }
  }
  /* Return error code if realloc() failed */
  return (buckets == NULL) ? -1 : 0;
}

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
  if(node == NULL)
    goto fail;
  if(m->nnodes >= m->nbuckets) {
    n = (m->nbuckets > 0) ? (m->nbuckets << 1) : 1;
    err = map_resize(m, n);
    if(err)
      goto fail;
  }
  map_addnode(m, node);
  m->nnodes++;
  return 0;
fail:
  if(node)
    free(node);
  return -1;
}
