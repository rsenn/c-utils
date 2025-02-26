#include "../fmt.h"

size_t
fmt_humank(char* dest, uint64 l) {
  char unit;
  size_t i;

  if(l < 1000)
    return fmt_ulong(dest, l);

  if(l > (uint64)1024 * (uint64)1024 * (uint64)1024 * (uint64)1024) {
    l = (l + ((uint64)1024 * (uint64)1024 * (uint64)1024 * (uint64)1024 / 20)) / ((uint64)1024 * (uint64)1024 * (uint64)1024 * (uint64)1024 / 10);
    unit = 'T';
  } else if(l > (uint64)1024 * (uint64)1024 * (uint64)1024) {
    l = (l + ((uint64)1024 * (uint64)1024 * (uint64)1024 / 20)) / ((uint64)1024 * (uint64)1024 * (uint64)1024 / 10);
    unit = 'G';
  } else if(l > (uint64)1024 * (uint64)1024) {
    l = (l + ((uint64)1024 * (uint64)1024 / 20)) / ((uint64)1024 * (uint64)1024 / 10);
    unit = 'M';
  } else {
    l = (l + (1024 / 20)) / (1024 / 10);
    unit = 'k';
  }

  if(!dest)
    return fmt_ulong(0, l) + 2;
  i = fmt_ulong(dest, l / 10);
  dest[i] = '.';
  dest[i + 1] = (char)((l % 10) + '0');
  dest[i + 2] = unit;
  return i + 3;
}
