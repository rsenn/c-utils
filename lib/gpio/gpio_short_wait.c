#define _DEFAULT_SOURCE 1
#define _POSIX_SOURCE 1
#include "../windoze.h"
#include "../gpio_internal.h"

#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <unistd.h>
#endif

void
gpio_short_wait(void) {
#if WINDOWS_NATIVE
  Sleep(250);
#else
  usleep(250000); /* 250ms. */
#endif
}
