#include "unit_test.h"
#include "../lib/gpio.h"
#include <string.h>

/* the module pokes the registers of a BCM2835/6/7 through this pointer; the
 * tests point it at ordinary memory and look at the words */
extern volatile unsigned int* gpio;
static volatile unsigned int regs[64];

#define GPFSEL1 regs[1]
#define GPSET0 regs[7]
#define GPSET1 regs[8]
#define GPCLR0 regs[10]
#define GPCLR1 regs[11]
#define GPLEV0 regs[13]
#define GPLEV1 regs[14]
#define GPPUD regs[37]
#define GPPUDCLK0 regs[38]

static void
fake_open(void) {
  memset((void*)regs, 0, sizeof regs);
  gpio = regs;
}

/*
 * gpio_set_pin_mode: three bits per pin in the function select registers, ten pins in each
 */
TEST(test_gpio_set_pin_mode) {
  fake_open();
  gpio_set_pin_mode(12, 1); /* output */
  ASSERT_EQ(1u << 6, GPFSEL1);
  gpio_set_pin_mode(10, 1);
  ASSERT_EQ((1u << 6) | 1u, GPFSEL1);
  gpio_set_pin_mode(12, 0); /* back to input, the neighbour keeps its mode */
  ASSERT_EQ(1u, GPFSEL1);
  gpio = NULL;
}

/*
 * gpio_write, gpio_read, gpio_set_output
 */
TEST(test_gpio_write_read_output) {
  fake_open();
  gpio_write(5, 1);
  ASSERT_EQ(1u << 5, GPSET0);
  gpio_write(5, 0);
  ASSERT_EQ(1u << 5, GPCLR0);
  gpio_write(40, 1); /* pins from 32 are in the second registers */
  ASSERT_EQ(1u << 8, GPSET1);
  gpio_write(40, 0);
  ASSERT_EQ(1u << 8, GPCLR1);

  GPLEV0 = 1u << 7;
  GPLEV1 = 1u << 3;
  ASSERT_NE(0, gpio_read(7));
  ASSERT_EQ(0, gpio_read(6));
  ASSERT_NE(0, gpio_read(35));
  ASSERT_EQ(0, gpio_read(34));

  fake_open();
  gpio_set_output(21, 1); /* pin 21: register 2, bits 3..5 */
  ASSERT_EQ(1u << 3, regs[2]);
  ASSERT_EQ(1u << 21, GPSET0);
  gpio_set_output(22, 0);
  ASSERT_EQ(1u << 22, GPCLR0);
  gpio = NULL;
}

/*
 * gpio_short_wait, gpio_set_input_resistor_pull, gpio_set_input_pull, gpio_set_input_pull_up,
 * gpio_set_input_pull_down (each waits a quarter second twice, as the chip wants)
 */
TEST(test_gpio_input_pull) {
  fake_open();
  GPFSEL1 = 7u << 3; /* pin 11 is something else */
  gpio_set_input_pull_up(11);
  ASSERT_EQ(0u, GPFSEL1 & (7u << 3)); /* an input now */
  ASSERT_EQ(0u, GPPUD);               /* the control words are cleared again */
  ASSERT_EQ(0u, GPPUDCLK0);
  gpio_set_input_pull_down(11);
  ASSERT_EQ(0u, GPPUD);
  gpio_short_wait();
  gpio = NULL;
}

/*
 * gpio_init: without /dev/mem access (or off a Raspberry Pi) it fails and says so
 */
TEST(test_gpio_init_fails_cleanly) {
  gpio = NULL;
  /* on real hardware it would map the registers; either answer is allowed, only no crash */
  {
    int r = gpio_init();

    ASSERT_EQ(1, r == 0 || r == 1);
  }
  gpio = NULL;
}

#define RUN_GPIO_TESTS() \
  RUN(test_gpio_set_pin_mode); \
  RUN(test_gpio_write_read_output); \
  RUN(test_gpio_input_pull); \
  RUN(test_gpio_init_fails_cleanly)

TESTS(gpio) { RUN_GPIO_TESTS(); }
