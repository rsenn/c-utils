#include "../str.h"
/*
  return index to last occurance of data,
  otherwise return str_len(s)
*/
size_t
str_rfind(const void* s, const void* what) {
  return str_rfindb(s, what, str_len(what));
}

