#include "../str.h"

char*
str_basename(char* s) {
  size_t n = str_rchr(s, '/');
  if(s[n] != '\0') s = s + n + 1;
  return s;
}