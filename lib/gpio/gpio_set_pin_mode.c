#include "../gpio_internal.h"

void
gpio_set_pin_mode(int const pin_nr, int const mode) {
  unsigned int offset, shift;
  assert(gpio != NULL);
  assert(mode == GPIO_INPUT || mode == GPIO_OUTPUT);

   offset = pin_nr / 10; /* FPSEL register offset. */
      shift = (pin_nr % 10) * 3;           /* Bit shift. */

  GPFSEL[offset] &= ~(7 << shift); /* Clears bits. */

  GPFSEL[offset] |= mode << shift; /* Set mode/function. */
}
