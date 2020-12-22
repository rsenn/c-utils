#include "../byte.h"
#include "../bmap_internal.h"
#include "../alloc.h"

#include <stdlib.h>

static bmap_node_t*
bmap_newnode(const char* key, void* value, int vsize) {
  bmap_node_t* node;
  int ksize = str_len(key) + 1;
  int voffset = ksize + ((sizeof(void*) - ksize) % sizeof(void*));
  node = (bmap_node_t*)alloc(sizeof(*node) + voffset + vsize);
  if(!node)
    return NULL;
  byte_copy(node + 1, ksize, key);
  node->hash = bmap_hash(key);
  node->value = ((char*)(node + 1)) + voffset;
  byte_copy(node->value, vsize, value);
  return node;
}

static void
bmap_addnode(bmap_base_t* m, bmap_node_t* node) {
  int n = bmap_bucketidx(m, node->hash);
  node->next = m->buckets[n];
  m->buckets[n] = node;
}

static int
bmap_resize(bmap_base_t* m, int nbuckets) {
  bmap_node_t *nodes, *node, *next;
  bmap_node_t** buckets;
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
  buckets = m->buckets;
  alloc_re((void**)&buckets, sizeof(*m->buckets) * m->nbuckets, sizeof(*m->buckets) * nbuckets);
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
      bmap_addnode(m, node);
      node = next;
    }
  }
  /* Return error code if alloc_re() failed */
  return (buckets == NULL) ? -1 : 0;
}

int
bmap_set_(bmap_base_t* m, const char* key, void* value, int vsize) {
  int n, err;
  bmap_node_t **next, *node;
  /* Find & replace existing node */
  next = bmap_getref(m, key);
  if(next) {
    byte_copy((*next)->value, vsize, value);
    return 0;
  }
  /* Add new node */
  node = bmap_newnode(key, value, vsize);
  if(node == NULL)
    goto fail;
  if(m->nnodes >= m->nbuckets) {
    n = (m->nbuckets > 0) ? (m->nbuckets << 1) : 1;
    err = bmap_resize(m, n);
    if(err)
      goto fail;
  }
  bmap_addnode(m, node);
  m->nnodes++;
  return 0;
fail:
  if(node)
    free(node);
  return -1;
}
