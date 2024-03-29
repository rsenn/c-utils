#ifndef CBMAP_INTERNAL_H
#define CBMAP_INTERNAL_H

#include "cbmap.h"
#include "cbmap/cbmap_alloc.h"
#include <string.h>
#include <sys/types.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

/** A pair of allocation and deallocation functions */
typedef struct cbmap_allocator {
  cbmap_allocator_fn malloc;
  cbmap_deallocator_fn free;
} * cbmap_allocator_t;

struct cbmap {
  void* root;
  size_t count;
  struct cbmap_allocator key_allocator;
  struct cbmap_allocator value_allocator;
};

/** mark - cbmap_data_node : holds keys and values */

struct cbmap_data_node {
  unsigned char* key;
  unsigned char* value;
  size_t key_len;
  size_t value_len;
};

/** mark - cbmap_internal_node : holds structural information of nodes */

/**
 A cbmap_internal_node is a branching node on a given bit.
*/
struct cbmap_internal_node {
  /* The two branches */
  void* branch[2];
  /* The number of the byte where the bits differ */
  unsigned int byte;
  /* All ones but the branching bit, that is set to 0 */
  unsigned char otherbits;
};

#define KEY_COMPARE(k1, k2, len) byte_diff(k1, len, k2)
#define VALUE_COMPARE(k1, k2, len) byte_diff(k1, len, k2)

/* Return values */
#define INSERT_OUT_OF_MEMORY 0
#define INSERT_OK 1
#define INSERT_UPDATED 2
#define NOT_FOUND 0
#define FOUND 1

/** mark - cbmap */

#define IS_INTERNAL_NODE(p) (1 & (size_t)(p))
#define GET_INTERNAL_NODE(p) ((struct cbmap_internal_node*)(((size_t)(p)) - 1))

#define IS_DATA_NODE(p) (!(IS_INTERNAL_NODE(p)))
#define GET_DATA_NODE(p) ((struct cbmap_data_node*)(p))

struct cbmap_internal_node* cbmap_internal_node_new(void);

void cbmap_internal_node_destroy(struct cbmap_internal_node* node);
void cbmap_data_node_destroy(struct cbmap_data_node* data, cbmap_allocator_t key_allocator, cbmap_allocator_t value_allocator);
#endif
