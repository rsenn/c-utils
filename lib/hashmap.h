/**
 * @defgroup   hashmap
 * @brief      HASHMAP module.
 * @{
 */
#ifndef HASHMAP_H
#define HASHMAP_H

#include "typedefs.h"
#include "uint64.h"
#include "bool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  const char* key;
  size_t keylen;
  void* val;
} hashentry;

typedef struct {
  hashentry* buckets;
  size_t capacity, used;
} hashmap;

#define HASHMAP_INIT() \
  (hashmap) { 0, 0, 0 }

void hashmap_init(hashmap*);
void hashmap_delete2(hashmap*, const char*, size_t);
void hashmap_delete(hashmap*, const char*);
void* hashmap_get2(hashmap*, const char*, size_t);
void* hashmap_get(hashmap*, const char*);
hashentry* hashmap_get_entry(hashmap*, const char*, size_t);
hashentry* hashmap_get_or_insert_entry(hashmap*, const char*, size_t);
bool hashmap_match(hashentry*, const char*, size_t);
void hashmap_put2(hashmap*, const char*, size_t, void*);
void hashmap_put(hashmap*, const char*, void*);
void hashmap_rehash(hashmap*);

/* Initial hash bucket size*/
#define HASHMAP_INIT_SIZE 16

/* Rehash if the usage exceeds 70%.*/
#define HASHMAP_HIGH_WATERMARK 70

/* We'll keep the usage below 50% after rehashing.*/
#define HASHMAP_LOW_WATERMARK 50

/* Represents a deleted hash entry*/
#define HASHMAP_TOMBSTONE ((void*)-1)

static inline uint64
hashmap_fnv_hash(const char* s, size_t len) {
  uint64 hash = 0xcbf29ce484222325;

  for(size_t i = 0; i < len; i++) {
    hash *= 0x100000001b3;
    hash ^= (unsigned char)s[i];
  }

  return hash;
}

#ifdef __cplusplus
}
#endif

#endif /* defined HASHMAP_H */
/** @} */
