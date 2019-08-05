#include "../gpio_internal.h"

void
gpio_set_input_pull_up(int const pin_nr) {
  gpio_set_input_pull(pin_nr, GPIO_PULL_UP);
}

