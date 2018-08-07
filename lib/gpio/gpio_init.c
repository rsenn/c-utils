#include "../buffer.h"
#include "../open.h"
#include "../gpio_internal.h"

volatile unsigned int* gpio = NULL; /* See gpio_init(). */

bool
gpio_init() {
  int mem_dev = -1;
  void* mem_map = NULL;

  if(gpio != NULL) {
    buffer_puts(buffer_1, "gpio_init : Error: Seems to be already initialized!");
    buffer_putnlflush(buffer_1);
    return false;
  }

  mem_dev = open_rwsync("/dev/mem");
  if(mem_dev == -1) {
    buffer_puts(buffer_1, "gpio_init : Error: Can't open \"/dev/mem\"!");
    buffer_putnlflush(buffer_1);
    return false;
  }

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
    return false;
  }

  gpio = (volatile unsigned*)mem_map;
  return true;

  /* [munmap() call is not necessary later, */
  /* because it will automatically unmap on process termination] */
}
