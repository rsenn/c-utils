/**
 * @defgroup   set
 * @brief      Set of byte strings that remembers the order of insertion.
 * @{
 *
 * A set holds values of any length, strings or binary data, each once. The set
 * copies what it is given, so a value can be a temporary, a stralloc or a
 * piece of a longer text. A copy ends in a NUL byte that its size does not
 * count, so every value can also be used as a C string.
 *
 * Values are found by hash and kept in the order in which they were added:
 * iteration, the functions that combine sets, and the text made by set_join
 * all follow that order.
 *
 *     set_t s = SET();                  // an empty set, nothing to start
 *
 *     set_adds(&s, "gcc");
 *     set_adds(&s, "ar");
 *     set_adds(&s, "gcc");              // SET_EXISTS: it keeps its place
 *
 *     if(set_has_s(&s, "ar")) ...
 *
 *     {
 *       set_iterator_t it;
 *       const char* x;
 *       size_t n;
 *
 *       set_foreach(&s, it, x, n)       // "gcc", then "ar"
 *         use(x, n);
 *     }
 *
 *     set_free(&s);                     // gives the memory back, the set is
 *                                       // a new SET() again
 *
 * A function that can run out of memory says so and leaves the set as it was.
 */

#ifndef SET_H
#define SET_H

#include "uint32.h"
#include "stralloc.h"
#include "buffer.h"

/** Results of set_add and set_insert */
#define SET_ERROR 0  /**< the memory ran out, the set is unchanged */
#define SET_ADDED 1  /**< the value is new, it was added */
#define SET_EXISTS 2 /**< the set has the value already, it stays where it is */

/** One value of a set. Read its fields, never write them. */
typedef struct bucket {
  struct bucket* next;      /**< the next one with the same hash slot */
  struct bucket* list_next; /**< the next one in the order of insertion */
  struct bucket* list_prev; /**< the previous one in the order of insertion */
  void* value;              /**< the value, followed by a NUL byte */
  size_t size;              /**< number of bytes in value */
  uint32 hash;
} bucket_t;

/** A set. Read its fields, never write them (set_size is the number of values). */
typedef struct set {
  uint32 entries;  /**< number of values */
  uint32 overflow; /**< number of values that are not in a slot of the table */
  uint32 len;      /**< number of slots of the table, a power of two */
  uint32 (*hash_fp)(const void*, size_t);
  bucket_t *array, *list, *last;
} set_t;

/** Position in a set. */
typedef struct set_iterator {
  uint32 index; /**< how many values the iterator has passed */
  const bucket_t* current;
  const set_t* set;
} set_iterator_t;

/** Initializer of an empty set that is ready to use: `set_t s = SET();` */
#define SET() \
  { 0, 0, 0, 0, 0, 0, 0 }

/* ---- start and end ------------------------------------------------------ */

/** Makes a set that was never used empty, as `*s = SET()` does. It does not
 * look at what the memory held: use set_free for a set that has values. */
void set_init(set_t*);

/** Removes all values, keeps the memory for the next ones. */
void set_clear(set_t*);

/** Releases all memory. The set is empty afterwards and can be used again. */
void set_free(set_t*);

/* ---- adding and removing ------------------------------------------------ */

/** Adds the size bytes at val after the others: SET_ADDED, SET_EXISTS or SET_ERROR. */
int set_add(set_t*, const void* val, const size_t size);
/** Adds a string, without its NUL, after the others. */
int set_adds(set_t*, const char* s);
/** Adds the contents of a stralloc after the others. */
int set_addsa(set_t*, const stralloc* sa);

/** Like set_add, but a new value goes before the others. */
int set_insert(set_t*, const void* val, const size_t size);

/** Takes a value out: 1 if it was there, 0 if not. */
int set_delete(set_t*, const void* val, const size_t size);

/* ---- asking ------------------------------------------------------------- */

/** 1 if the set has the value, otherwise 0. */
int set_has(const set_t*, const void* val, const size_t size);
int set_has_s(const set_t*, const char* s);
int set_has_sa(const set_t*, const stralloc* sa);

/** Number of values. */
static inline uint32
set_size(const set_t* s) {
  return s->entries;
}

/** The first and the last value in the order, and their size if len is not 0;
 * NULL if the set is empty. */
char* set_first(const set_t*, size_t* len);
char* set_last(const set_t*, size_t* len);

/* ---- combining ---------------------------------------------------------- */

/* These add to the destination, which may hold values already, in the order of
 * the source. They return how many values they added, or -1 if the memory
 * ran out. */

/** set gets the values of other that it does not have. */
int set_cat(set_t* set, const set_t* other);
/** out gets the values that both a and b have. */
int set_intersection(set_t* out, const set_t* a, const set_t* b);
/** out gets the values for which pred(value, size) is true. */
int set_filter(const set_t* in, set_t* out, int (*pred)(const char*, size_t));
/** out gets the values for which pred(value, size) is false. */
int set_filter_out(const set_t* in, set_t* out, int (*pred)(const char*, size_t));

/* ---- walking ------------------------------------------------------------ */

/** Walks the values in the order of insertion, x is the value and n its size:
 *
 *     set_iterator_t it;
 *     const char* x;
 *     size_t n;
 *
 *     set_foreach(&s, it, x, n) ...
 *
 * The set must not change inside the loop. */
#define set_foreach(s, it, x, n) \
  for(it = set_iterator_new((s)); (x = set_iterator_value(&(it), &n)); set_iterator_next(&(it)))

/** Walks the buckets (value, size, hash and neighbours): b is a bucket_t*. */
#define set_foreach_ordered(s, b, x, n) for(b = (s)->list; b && (n = b->size, x = b->value); b = b->list_next)

/** The parts that set_foreach is made of. */
int set_iterator_init(set_iterator_t*, const set_t*);
int set_iterator_next(set_iterator_t*);
char* set_iterator_value(const set_iterator_t*, size_t* size);

static inline set_iterator_t
set_iterator_new(const set_t* s) {
  set_iterator_t it;
  set_iterator_init(&it, s);
  return it;
}

/* ---- text --------------------------------------------------------------- */

/** Appends the values with a separator between them to out: 1, or 0 if the memory ran out. */
int set_join(const set_t*, const char* sep, stralloc* out);

/** The same as a new string that malloc made, NULL if the memory ran out. */
char* set_join_s(const set_t*, const char* sep);

/** Writes the values with a separator to a buffer. Trailing NUL bytes of a
 * value are left out. */
void buffer_putset(buffer*, const set_t*, const char* sep, size_t seplen);

#endif /* defined SET_H */
/** @} */
