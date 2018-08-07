#include "../gpio_internal.h"

void
gpio_set_input_resistor_pull(int const pin_nr, int const pull) {
  assert(gpio != NULL);
  assert(pull == GPIO_PULL_DOWN || pull == GPIO_PULL_UP);

  *GPPULL = pull;
  gpio_short_wait();

  *GPPULLCLK0 = 1 << pin_nr; /* Clock on GPIO pin (bit set). */
  gpio_short_wait();

  *GPPULL = 0;
  *GPPULLCLK0 = 0;
}
