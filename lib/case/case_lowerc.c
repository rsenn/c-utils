#include "../case.h"

int
case_lowerc(int c) {
  if(c >= 'A' && c <= 'Z')
    c += 'a' - 'A';
  return c;
}
