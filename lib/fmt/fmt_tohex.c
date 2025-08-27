#include "../fmt.h"

    const char fmt_hexchars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

char
fmt_tohex(char c) {
  return fmt_hexchars[c&0xf];
}
