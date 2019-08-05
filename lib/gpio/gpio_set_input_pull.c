#include "../gpio_internal.h"

void
gpio_set_input_pull(int const pin_nr, int const pull) {
  gpio_set_pin_mode(pin_nr, GPIO_INPUT);
  gpio_set_input_resistor_pull(pin_nr, pull);
}

