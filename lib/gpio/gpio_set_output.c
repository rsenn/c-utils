#include "../gpio_internal.h"

void
gpio_set_output(int const pin_nr, int const high) {
  gpio_set_pin_mode(pin_nr, GPIO_OUTPUT);
  gpio_write(pin_nr, high);
}
