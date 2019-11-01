#include "../str.h"
#include "../byte.h"

char*
str_triml(char* s, const char* charset, unsigned int charsetlen) {
  while(*s) {
    if(byte_chr(charset, charsetlen, *s) != charsetlen)
      break;
    ++s;
  }
  return s;
}
