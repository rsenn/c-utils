#include "../str.h"
/*
  return index to first occurance of data,
  otherwise return str_len(s)
*/
size_t
str_find(const void* s, const void* what) {
  return str_findb(s, what, str_len(what));
}
