#include "unit_test.h"
#include "../lib/hashmap.h"
#include "../lib/fmt.h"
#include "../lib/str.h"

/* Builds "key<i>" into buf (which must be large enough) and returns its
 * length, without pulling in a printf-style formatter. */
static size_t
make_key(char* buf, unsigned long i) {
  size_t n = str_copy(buf, "key");
  n += fmt_ulong(buf + n, i);
  buf[n] = 0;
  return n;
}

/*
 * void hashmap_init(hashmap*);
 * void hashmap_put(hashmap*, const char*, void*);
 * void* hashmap_get(hashmap*, const char*);
 */
TEST(test_hashmap_put_get) {
  hashmap map = HASHMAP_INIT();

  hashmap_put(&map, "foo", (void*)1);
  hashmap_put(&map, "bar", (void*)2);

  ASSERT_EQ(1, (size_t)hashmap_get(&map, "foo"));
  ASSERT_EQ(2, (size_t)hashmap_get(&map, "bar"));
  ASSERT_EQ(2, map.used);
}

TEST(test_hashmap_get_missing) {
  hashmap map = HASHMAP_INIT();

  /* empty map: no buckets allocated yet, hashmap_get_entry() must take
   * the !map->buckets early-out rather than dividing by zero capacity */
  ASSERT_EQ(NULL, hashmap_get(&map, "missing"));

  hashmap_put(&map, "foo", (void*)1);
  ASSERT_EQ(NULL, hashmap_get(&map, "bar"));
}

TEST(test_hashmap_overwrite) {
  hashmap map = HASHMAP_INIT();

  hashmap_put(&map, "key", (void*)1);
  hashmap_put(&map, "key", (void*)2);

  ASSERT_EQ(2, (size_t)hashmap_get(&map, "key"));
  ASSERT_EQ(1, map.used); /* overwriting an existing key must not grow used */
}

/*
 * void hashmap_delete(hashmap*, const char*);
 */
TEST(test_hashmap_delete) {
  hashmap map = HASHMAP_INIT();

  hashmap_put(&map, "foo", (void*)1);
  hashmap_put(&map, "bar", (void*)2);
  hashmap_delete(&map, "foo");

  ASSERT_EQ(NULL, hashmap_get(&map, "foo"));
  ASSERT_EQ(2, (size_t)hashmap_get(&map, "bar"));
}

TEST(test_hashmap_delete_missing) {
  hashmap map = HASHMAP_INIT();

  /* deleting from an empty map, or a key that was never present, must
   * not crash */
  hashmap_delete(&map, "nope");

  hashmap_put(&map, "foo", (void*)1);
  hashmap_delete(&map, "nope");
  ASSERT_EQ(1, (size_t)hashmap_get(&map, "foo"));
}

TEST(test_hashmap_reuse_tombstone) {
  hashmap map = HASHMAP_INIT();

  /* re-inserting the SAME key after deleting it starts its probe at the
   * same bucket, so it deterministically lands on its own tombstone --
   * unlike a different key, whose probe starts wherever its own hash
   * happens to land, with no guarantee of hitting that same slot */
  hashmap_put(&map, "foo", (void*)1);

  size_t used_before = map.used;

  hashmap_delete(&map, "foo");
  hashmap_put(&map, "foo", (void*)2);

  /* reusing the tombstoned slot must not grow `used` */
  ASSERT_EQ(used_before, map.used);
  ASSERT_EQ(2, (size_t)hashmap_get(&map, "foo"));
}

/*
 * void hashmap_put2(hashmap*, const char*, size_t, void*);
 * void* hashmap_get2(hashmap*, const char*, size_t);
 */
TEST(test_hashmap_binary_keys) {
  hashmap map = HASHMAP_INIT();
  char k1[] = {'a', '\0', 'b'};
  char k2[] = {'a', '\0', 'c'};

  /* keys with embedded NULs must be distinguished by their explicit
   * length, not truncated at the first NUL like the str_len()-based
   * hashmap_put()/hashmap_get() wrappers would */
  hashmap_put2(&map, k1, sizeof(k1), (void*)1);
  hashmap_put2(&map, k2, sizeof(k2), (void*)2);

  ASSERT_EQ(1, (size_t)hashmap_get2(&map, k1, sizeof(k1)));
  ASSERT_EQ(2, (size_t)hashmap_get2(&map, k2, sizeof(k2)));
  ASSERT_EQ(NULL, hashmap_get(&map, "a")); /* str_len("a") == 1, a distinct key */
}

/*
 * hashentry* hashmap_get_entry(hashmap*, const char*, size_t);
 * bool hashmap_match(hashentry*, const char*, size_t);
 */
TEST(test_hashmap_match) {
  hashentry ent = {0};
  char key[] = "abc";

  ent.key = key;
  ent.keylen = 3;

  ASSERT_NE(0, hashmap_match(&ent, "abc", 3));
  ASSERT_EQ(0, hashmap_match(&ent, "abd", 3)); /* same length, different bytes */
  ASSERT_EQ(0, hashmap_match(&ent, "abc", 2)); /* same bytes, different length */

  ent.key = HASHMAP_TOMBSTONE;
  ASSERT_EQ(0, hashmap_match(&ent, "abc", 3));

  ent.key = NULL;
  ASSERT_EQ(0, hashmap_match(&ent, "abc", 3));
}

/*
 * uint64 hashmap_fnv_hash(const char*, size_t);
 */
TEST(test_hashmap_fnv_hash) {
  /* deterministic: the same input always hashes the same */
  ASSERT_EQ(hashmap_fnv_hash("abc", 3), hashmap_fnv_hash("abc", 3));

  /* different input (near-certainly) hashes differently -- not a proof
   * of good distribution, just a smoke test that it looks at its input */
  ASSERT_NE(hashmap_fnv_hash("abc", 3), hashmap_fnv_hash("abd", 3));
  ASSERT_NE(hashmap_fnv_hash("", 0), hashmap_fnv_hash("a", 1));
}

/*
 * void hashmap_rehash(hashmap*);
 *
 * Exercised indirectly through hashmap_put2()'s HASHMAP_HIGH_WATERMARK
 * check rather than called directly -- growing the map is the only way
 * callers ever trigger it.
 */
TEST(test_hashmap_grows_and_keeps_all_keys) {
  hashmap map = HASHMAP_INIT();
  enum { N = 5000 };
  size_t capacity_before;

  hashmap_put(&map, "seed", (void*)(size_t)~0ul);
  capacity_before = map.capacity;

  for(unsigned long i = 0; i < N; i++) {
    char key[32];

    make_key(key, i);
    hashmap_put(&map, key, (void*)(size_t)(i + 1));
  }

  /* HASHMAP_INIT_SIZE is 16 and HASHMAP_HIGH_WATERMARK is 70%, so 5000
   * entries must have forced at least one hashmap_rehash() */
  ASSERT_GT(map.capacity, capacity_before);
  ASSERT_EQ(N + 1, map.used);

  for(unsigned long i = 0; i < N; i++) {
    char key[32];

    make_key(key, i);
    ASSERT_EQ(i + 1, (size_t)hashmap_get(&map, key));
  }

  ASSERT_EQ((size_t)~0ul, (size_t)hashmap_get(&map, "seed"));
  ASSERT_EQ(NULL, hashmap_get(&map, "no such key"));
}

TEST(test_hashmap_delete_survives_rehash) {
  hashmap map = HASHMAP_INIT();
  enum { N = 2000 };

  for(unsigned long i = 0; i < N; i++) {
    char key[32];

    make_key(key, i);
    hashmap_put(&map, key, (void*)(size_t)(i + 1));
  }

  /* delete every other key, then grow the map further -- hashmap_rehash()
   * must drop the tombstones rather than resurrecting them */
  for(unsigned long i = 0; i < N; i += 2) {
    char key[32];

    make_key(key, i);
    hashmap_delete(&map, key);
  }

  for(unsigned long i = N; i < N + 3000; i++) {
    char key[32];

    make_key(key, i);
    hashmap_put(&map, key, (void*)(size_t)(i + 1));
  }

  for(unsigned long i = 0; i < N; i++) {
    char key[32];

    make_key(key, i);

    if(i % 2 == 0) {
      ASSERT_EQ(NULL, hashmap_get(&map, key));
    } else {
      ASSERT_EQ(i + 1, (size_t)hashmap_get(&map, key));
    }
  }

  for(unsigned long i = N; i < N + 3000; i++) {
    char key[32];

    make_key(key, i);
    ASSERT_EQ(i + 1, (size_t)hashmap_get(&map, key));
  }
}

#define RUN_HASHMAP_TESTS() \
  RUN(test_hashmap_put_get); \
  RUN(test_hashmap_get_missing); \
  RUN(test_hashmap_overwrite); \
  RUN(test_hashmap_delete); \
  RUN(test_hashmap_delete_missing); \
  RUN(test_hashmap_reuse_tombstone); \
  RUN(test_hashmap_binary_keys); \
  RUN(test_hashmap_match); \
  RUN(test_hashmap_fnv_hash); \
  RUN(test_hashmap_grows_and_keeps_all_keys); \
  RUN(test_hashmap_delete_survives_rehash)

TESTS(hashmap) { RUN_HASHMAP_TESTS(); }
