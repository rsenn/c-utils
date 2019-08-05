#include "../gpio_internal.h"

void
gpio_write(int const pin_nr, int const high) {
  assert(gpio != NULL);

  if(high) {
    GPSET[PIN_OFFSET(pin_nr)] = PIN_VAL(pin_nr);
    return;
  }
  GPCLR[PIN_OFFSET(pin_nr)] = PIN_VAL(pin_nr);
}

