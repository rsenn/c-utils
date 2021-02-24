#define MAP_USE_HMAP 1
#include "uint64.h"
#include "buffer.h"
#include "map.h"
#include "json.h"
#include "bool.h"
#include <ctype.h>

typedef int ctypefn_t(int);
typedef ctypefn_t* ctypefn_ptr;

static inline int
predicate_ctype(int c, size_t pos, void* ptr) {
  ctypefn_ptr fn = ptr;
  return fn(c);
}

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
  const char* str = ptr;
    return pos < str_len(str) ? predicate_string(c, pos, str)  : !is_identifier_char(c);

 }
static inline int
predicate_null(int c, size_t pos, void* ptr) {
    return predicate_identifier(c, pos, "null");
    }