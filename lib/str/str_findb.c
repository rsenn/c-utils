#include "../byte.h"
#include "../str.h"
/*
  return index to first occurance of data,
  otherwise return str_len(s)
*/
size_t
str_findb(const void* s, const void* what, size_t len) {
  return byte_findb(s, str_len(s), what, len);
}
