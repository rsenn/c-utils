#include "../taia.h"
#include "../tai.h"

double
taia_approx(const struct taia* t) {
  return tai_approx(&t->sec) + taia_frac(t);
}

