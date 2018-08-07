#include "../gpio_internal.h"

void
gpio_short_wait() {
  usleep(250000); /* 250ms. */
}
