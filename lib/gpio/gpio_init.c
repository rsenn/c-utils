#include "../windoze.h"
#include "../buffer.h"
#include "../open.h"
#include "../gpio_internal.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#endif

volatile unsigned int* gpio = NULL; /* See gpio_init(). */

int
gpio_init() {
  int mem_dev = -1;
  void* mem_map = NULL;

  if(gpio != NULL) {
    buffer_puts(buffer_1, "gpio_init : Error: Seems to be already initialized!");
    buffer_putnlflush(buffer_1);
    return 0;
  }

  mem_dev = open_rwsync("/dev/mem");
  if(mem_dev == -1) {
    buffer_puts(buffer_1, "gpio_init : Error: Can't open \"/dev/mem\"!");
    buffer_putnlflush(buffer_1);
    return 0;
  }
#if !(defined(_WIN32) || defined(_WIN64))
  mem_map = mmap(NULL,                   /* Local mapping start address (NULL means don't care). */
                 BLOCK_SIZE,             /* Mapped memory block size. */
                 PROT_READ | PROT_WRITE, /* Enable read and write. */
                 MAP_SHARED,             /* No exclusive access. */
                 mem_dev,
                 GPIO_BASE); /* Offset to GPIO peripheral. */
  close(mem_dev);
  mem_dev = -1;

  if(mem_map == MAP_FAILED) {
    buffer_puts(buffer_1, "gpio_init : Error: Failed to create memory mapping!");
    buffer_putnlflush(buffer_1);
    return 0;
  }
#endif

  gpio = (volatile unsigned*)mem_map;
  return 1;

  /* [munmap() call is not necessary later, */
  /* because it will automatically unmap on process termination] */
}
