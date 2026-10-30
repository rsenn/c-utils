#include "../ucs_internal.h"

/* ucs_punycode_adapt returns the new bias after a delta was encoded. */
uint32
ucs_punycode_adapt(uint32 delta, uint32 numpoints, int first) {
  uint32 k = 0;

  delta = first ? delta / 700 : delta / 2;
  delta += delta / numpoints;

  while(delta > ((PUNYCODE_BASE - PUNYCODE_TMIN) * PUNYCODE_TMAX) / 2) {
    delta /= PUNYCODE_BASE - PUNYCODE_TMIN;
    k += PUNYCODE_BASE;
  }
  return k + (PUNYCODE_BASE - PUNYCODE_TMIN + 1) * delta / (delta + 38);
}
