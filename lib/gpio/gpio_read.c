#include "../gpio_internal.h"

int
gpio_read(int const pin_nr) {
  assert(gpio != NULL);

  return GPLEV[PIN_OFFSET(pin_nr)] & PIN_VAL(pin_nr);
}
