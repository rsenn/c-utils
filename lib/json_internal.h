#define MAP_USE_HMAP 1
#include "uint64.h"
#include "buffer.h"
#include "map.h"
#include "json.h"
#include "bool.h"
#include <ctype.h>

static inline int
predicate_string(int c, size_t pos, void* ptr) {
  const char* str = ptr;
  return str[pos] && str[pos] == c;
}

static inline int
is_identifier_char(int c) {
  static const char identifier_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789$_";
  return !!identifier_chars[str_chr(identifier_chars, c)];
}

static inline int
predicate_identifier(int c, size_t pos, void* ptr) {
  char* str = ptr;
  size_t len = str_len(str);

  return pos < len ? predicate_string(c, pos, str) : pos == len ? !is_identifier_char(c) : 0;
}
static inline int
predicate_null(int c, size_t pos, void* ptr) {
  return predicate_identifier(c, pos, "null");
}
