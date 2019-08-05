#include "../str.h"
#include <string.h>

char*
str_tok(char* s, const char* delims, char** ptrptr) {
  char* ret;

  if(s == NULL) { 
    s = *ptrptr;
    while(*s && strchr(delims, *s)) ++s;
  }

  if(*s == '\0') return NULL;

  ret = s;

  while(*s && !strchr(delims, *s)) ++s;

  if(*s) *s++ = '\0';

  *ptrptr = s;
  return ret;
}

