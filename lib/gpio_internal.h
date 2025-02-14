#include "gpio.h"

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include "mmap.h"

/* Physical addresses */
#define BCM2835_PERI_BASE 0x20000000 /* Raspi 1 */
#define BCM2836_PERI_BASE 0x3F000000 /* Raspi 2 */

#define PERI_BASE BCM2836_PERI_BASE

#define GPIO_BASE (PERI_BASE + 0x200000)
#define BLOCK_SIZE (4 * 1024)

static const int GPIO_INPUT = 0;
static const int GPIO_OUTPUT = 1;

static const int GPIO_PULL_DOWN = 1;
static const int GPIO_PULL_UP = 2;

#define GPFSEL ((volatile unsigned int*)(gpio + 0))
#define GPSET ((volatile unsigned int*)(gpio + 7))
#define GPCLR ((volatile unsigned int*)(gpio + 10))
#define GPLEV ((volatile unsigned int*)(gpio + 13))
#define GPPULL \
  ((volatile unsigned int*)(gpio + 37)) /* Pull up / pull down. \
                                         */
#define GPPULLCLK0 \
  ((volatile unsigned int*)(gpio + 38)) /* Pull up / pull down clock. */

#define PIN_OFFSET(n) (n < 32 ? 0 : 1) /* Register offset. */
#define PIN_VAL(n) (1 << (n % 32))

extern volatile unsigned int* gpio; /* See gpio_init(). */
