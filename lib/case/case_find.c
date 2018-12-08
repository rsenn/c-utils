#include "../case.h"
/*
  return index to first occurance of data,
  otherwise return str_len(s)
*/
size_t
case_find(const void* s, const void* what) {
  return case_findb(s, what, str_len(what));
}
