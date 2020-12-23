/**
 * @defgroup   bmap
 * @brief      BMAP module.
 * @{
 */
#ifndef _BMAP_H
#define _BMAP_H

#include "byte.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAP_VERSION "0.1.0"

struct bmap_node_s;
typedef struct bmap_node_s bmap_node_t;

typedef struct {
  bmap_node_t** buckets;
  unsigned nbuckets, nnodes;
} bmap_base_t;

typedef struct {
  unsigned bucketidx;
  bmap_node_t* node;
} bmap_iter_t;

#define bmap_t(T)                                                                                                                                                                                      \
  struct {                                                                                                                                                                                             \
    bmap_base_t base;                                                                                                                                                                                  \
    T* ref;                                                                                                                                                                                            \
    T tmp;                                                                                                                                                                                             \
  }

#define bmap_init(m) byte_zero(m, sizeof(*(m)))

#define bmap_deinit(m) bmap_deinit_(&(m)->base)

#define bmap_get(m, key) ((m)->ref = bmap_get_(&(m)->base, (key)))

#define bmap_set(m, key, value)                                                                                                                                                                        \
  do {                                                                                                                                                                                                 \
    (m)->tmp = (value);                                                                                                                                                                                \
    bmap_set_(&(m)->base, key, (void*)&(m)->tmp, sizeof((m)->tmp));                                                                                                                                    \
  } while(0)

#define bmap_remove(m, key) bmap_remove_(&(m)->base, key)

#define bmap_iter(m) bmap_iter_()

#define bmap_next(m, iter) bmap_next_(&(m)->base, iter)

void bmap_deinit_(bmap_base_t* m);
void* bmap_get_(bmap_base_t* m, const char* key);
int bmap_set_(bmap_base_t* m, const char* key, void* value, int vsize);
void bmap_remove_(bmap_base_t* m, const char* key);
bmap_iter_t bmap_iter_(void);
const char* bmap_next_(bmap_base_t* m, bmap_iter_t* iter);

typedef bmap_t(void*) bmap_void_t;
typedef bmap_t(char*) bmap_str_t;
typedef bmap_t(int) bmap_int_t;
typedef bmap_t(char) bmap_char_t;
typedef bmap_t(float) bmap_float_t;
typedef bmap_t(double) bmap_double_t;

#ifdef __cplusplus
}
#endif
#endif
/** @} */
