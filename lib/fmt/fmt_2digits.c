#include "../fmt.h"
#include "../byte.h"
#include <time.h>

unsigned int
fmt_2digits(char* dest, int i) {
  dest[0] = (char)((i / 10) + '0');
  dest[1] = (char)((i % 10) + '0');
  return 2;
}
