/**
 * @defgroup   set
 * @brief      SET module.
 * @{
 */

#ifndef SET_H
#define SET_H

#include "uint32.h"

typedef struct bucket {
  struct bucket* next;
  void* value;
  size_t size;
  uint32 hash;
} bucket_t;

typedef struct set {
  uint32 entries;
  uint32 overflow;
  uint32 len;
  uint32 (*hash_fp)(const void*, size_t);
  bucket_t* array;
} set_t;

// Iterator Definition
typedef struct set_iterator {
  uint32 index;
  const bucket_t* current;
  const set_t* set;
} set_iterator_t;

size_t fmt_set(char*, const set_t*, const char*);

int set_add(set_t*, const void*, const size_t);
int set_adds(set_t*, const char*);
void set_clear(set_t*);
int set_delete(set_t*, const void*, const size_t);
void set_free_array(set_t*);
void set_free(set_t*);
int set_has(const set_t*, const void*, const size_t);
uint32 set_hashfunc(const void*, size_t);
void set_init(set_t*, uint32 (*const)(const void*, size_t));
int set_cat(set_t*, const set_t*);

int set_sub(const set_t* a, const set_t* b, set_t* c);
int set_filter(const set_t* in, set_t* out, int (*pred)(const char*, size_t));
int set_filter_out(const set_t* in, set_t* out, int (*pred)(const char*, size_t));

int set_iterator_erase(set_iterator_t*);
int set_iterator_find(set_iterator_t*, const void*, size_t);
int set_iterator_init(set_iterator_t*, const set_t*);
int set_iterator_next(set_iterator_t*);
char* set_iterator_value(const set_iterator_t*, size_t*);

char** set_toarray(const set_t*);

char* set_at(const set_t*, const size_t pos);
char* set_at_n(const set_t*, const size_t pos, size_t* len);

#define set_hash(s, x, len) ((s)->hash_fp((x), (len)))

static inline uint32
set_size(const set_t* s) {
  return s->entries;
}

int set_iterator_init(set_iterator_t*, const set_t*);
int set_iterator_next(set_iterator_t*);
int set_iterator_erase(set_iterator_t*);
char* set_iterator_value(const set_iterator_t*, size_t*);

static inline set_iterator_t
set_iterator_new(const set_t* s) {
  set_iterator_t it;
  set_iterator_init(&it, s);
  return it;
}

#define set_foreach_it(s, it) for(set_iterator_init(&(it), (s)); set_iterator_value(&(it), 0); set_iterator_next(&(it)))

#define set_foreach(s, it, x, n)                                                                                       \
  for(it = set_iterator_new((s)); (x = set_iterator_value(&(it), &n)); set_iterator_next(&(it)))

#ifdef STRALLOC_H
int set_has_sa(const set_t*, const stralloc*);
int set_addsa(set_t*, const stralloc*);
size_t stralloc_catset(stralloc*, const set_t*, const char*);
void set_join(const set_t* set, const char* sep, stralloc* out);
#endif
#ifdef BUFFER_H
void buffer_putset(buffer*, const set_t*, const char* sep, size_t seplen);
#endif

#endif
/** @} */
