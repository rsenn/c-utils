#include "../str.h"
/*
  return index to first occurance of data,
  otherwise return str_len(s)
*/
unsigned int str_find(const void *s, register const void *what)
{
  return str_findb(s, what, str_len(what));
}
