/**
 * @defgroup   cbmap
 * @brief      CBMAP module.
 * @{
 */
#ifndef _CBMAP_H
#define _CBMAP_H

#include <stdlib.h>
#include "uint8.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Handles allocation (and copying) of keys and values */
typedef void* (*cbmap_allocator_fn)(void* data, size_t data_len);

/** Handles deallocation of keys and values */
typedef void (*cbmap_deallocator_fn)(void* data);

/** @return 1 to continue iterating, any other to stop. */
typedef int (*cbmap_visitor)(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data);

/** Opaque map type */
typedef struct cbmap* cbmap_t;

/**
 Returns a map that allocates and deallocates keys using malloc and free.
 Keys and values may be changed during the map lifetime, because copies are
 not modified.
*/
cbmap_t cbmap_new(void);

/**
 Returns a map that does not allocate nor deallocate keys (just keeps
 pointers to them). WARNING: Of course, expect undefined behaviour if keys
 and values are changed during the map lifetime.
*/
cbmap_t cbmap_new_with_static_keys_and_values(void);

/**
 Returns a map with custom key allocation an deallocation functions.
 WARNING: Of course, expect undefined behaviour if keys and values are
 changed during the map lifetime.
*/
cbmap_t cbmap_new_with_custom_allocation(cbmap_allocator_fn key_allocator,
                                         cbmap_deallocator_fn key_deallocator,
                                         cbmap_allocator_fn value_allocator,
                                         cbmap_deallocator_fn value_deallocator);

/** Frees a map (invokes appropriate deallocators on keys and values) */
void cbmap_destroy(cbmap_t* cmap_ptr);

/** Returns the number of keys in the map */
size_t cbmap_count(cbmap_t map);

/** @return 0 if no memory, 1 on insertion and 2 on update. */
int cbmap_insert(cbmap_t map, void* key, size_t key_len, void* value, size_t value_len);

/** @return 0 if not found, 1 if found */
int cbmap_get(cbmap_t map, void* key, size_t key_len, void** value, size_t* value_len);

/** @return 0 if not found, 1 if found */
int cbmap_delete(cbmap_t map, void* key, size_t key_len);

/** Visits keys and values after a given key prefix */
int cbmap_visit(uint8* top, cbmap_visitor visitor_fn, void* user_data);

int cbmap_visit_prefix(cbmap_t map, uint8* key_prefix, size_t key_prefix_len, cbmap_visitor visitor_fn, void* user_data);

/** Visits all keys and values */
int cbmap_visit_all(cbmap_t map, cbmap_visitor visitor_fn, void* user_data);

#ifdef __cplusplus
}
#endif
#endif /* _CBMAP_H */
/** @} */
