/**
 * @defgroup   hashmap
 * @brief      HASHMAP module.
 * @{
 */
/**
 * @file
 * Simple hash map implementation.
 */

#ifndef HASHMAP_H
#define HASHMAP_H

#include "typedefs.h"
#include "linked_list.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Comparator function to determine whether `*l` and `*r` are equal.
 * @return Negative if `*l` is less than `*r`; zero if `*l` is equal to
 * `*r`; positive if `*l` is greater than `*r`.
 */
typedef int (*hashmap_comparator)(const void* l, const void* r);

/**
 * Hash function
 * @param key key to hash
 * @param capacity maximum size of the map
 * @return an offset within the range `[0, capacity)`
 */
typedef size_t (*hashmap_hash_func)(const void* key, size_t capacity);

typedef void* (*hashmap_key_dup_func)(const void* key);
typedef void (*hashmap_key_free_func)(const void* pair);

/**
 * Hash map object
 */
typedef struct {
  /** Maximum size of hash table */
  size_t capacity;
  /** Size of hash table */
  size_t size;
  /** Hash table */
  linked_list** table;
  /** Key comparator function */
  hashmap_comparator comparator;
  /** Key hash function */
  hashmap_hash_func hash_func;
  hashmap_key_dup_func key_dup_func;
  hashmap_key_free_func key_free_func;
  /** Keys */
  linked_list keys;
} hashmap;

/**
 * Key/value pair
 */
typedef struct {
  /** Key */
  void* key;
  /** Value */
  void* value;
} hashmap_pair;

/**
 * Initialize the hash map.
 * @param map hash map structure
 * @param capacity maximum size of the hash map
 * @param comparator key comparator function
 * @param hash_func key hash function
 */
void hashmap_init(hashmap* map,
                  size_t capacity,
                  hashmap_comparator comparator,
                  hashmap_hash_func hash_func,
                  hashmap_key_dup_func key_dup_func,
                  hashmap_key_free_func key_free_func);

/**
 * Free the hash map.
 * This function will also free the table of internal linked lists.
 * @param map hash map structure
 */
void hashmap_free(hashmap* map);

/**
 * Get the value for the given key.
 * @param map hash map structure
 * @param key key for value to fetch
 * @return pointer to the value
 */
void* hashmap_get(hashmap* map, void* key);

/**
 * Insert the value into the map.
 * @param map hash map structure
 * @param key key associated with value
 * @param value value associated with key
 */
linked_list_node* hashmap_put(hashmap* map, void* key, void* value);

/**
 * Remove the mapping from the map if this key exists. Calling this on
 * a key for which there is no mapping does nothing (does not error).
 * @param map hash map structure
 * @param key key for mapping to remove
 */
void hashmap_remove(hashmap* map, void* key);

/**
 * Returns number of key-value pairs in the map
 * @param map hash map structure
 * @return size of the hash map
 */
size_t hashmap_size(hashmap* map);

/**
 * Returns a linked list that contains all keys in the map
 * @param map hash map structure

 * @return a linked list containing all keys
 */
linked_list* hashmap_keys(hashmap* map);

/**
 * Removes all key/value pairs from the map
 * @param map hash map structure
 */
void hashmap_clear(hashmap* map);

/**
 * Check if the map contains the given key
 * @param map hash map structure
 * @param key the key to check
 * @return true if map contains key
 */
int hashmap_contains_key(hashmap* map, void* key);

int hashmap_default_comparator(const void*, const void* r);
size_t hashmap_default_hash_func(const void*, size_t capacity);

linked_list_node* hashmap_search(hashmap* map, void* key);

hashmap_pair* hashmap_find(hashmap* map, void* key);

#ifdef __cplusplus
}
#endif
#endif
/** @} */
