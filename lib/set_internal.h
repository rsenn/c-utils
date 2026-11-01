#ifndef SET_INTERNAL_H
#define SET_INTERNAL_H

#include "set.h"
#include "byte.h"

/* the hash of a value; a set uses byte_hash unless a test put another function in hash_fp */
#define set_hash(s, x, len) ((s)->hash_fp ? (s)->hash_fp((x), (len)) : byte_hash((x), (len)))

/* the bucket that holds the value, hash is set_hash of it; NULL if it is not there */
bucket_t* set_lookup(const set_t* set, const void* val, size_t size, uint32 hash);

/* adds a value at the end of the order (prepend 0) or at the front (prepend 1); returns SET_ADDED, SET_EXISTS or SET_ERROR */
int set_put(set_t* set, const void* val, size_t size, int prepend);

/* gives the table newlen slots (a power of two) and keeps the order and the values; 0 if the memory ran out, then the set is unchanged */
int set_rehash(set_t* set, uint32 newlen);

#endif /* defined SET_INTERNAL_H */
