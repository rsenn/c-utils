#ifndef GPIO_H
#define GPIO_H

/* RhinoDevel, Marcel Timm, 2017dec28 */


int gpio_read(int const pin_nr);
void gpio_set_input_pull(int const pin_nr, int const pull);
void gpio_set_input_resistor_pull(int const pin_nr, int const pull);
void gpio_set_pin_mode(int const pin_nr, int const mode);
void gpio_short_wait(void);

/** Initialize memory mapped Raspberry PI 2 stuff.
 */
int gpio_init();

/** Set pin with given nr. to output mode.
 *  Use int parameter value true for initial high level,
 *  false for initial low level output.
 */
void gpio_set_output(int const pin_nr, int const high);

/** Set pin with given nr. to input mode with pull up resistor.
 */
void gpio_set_input_pull_up(int const pin_nr);

/** Set pin with given nr. to input mode with pull down resistor.
 */
void gpio_set_input_pull_down(int const pin_nr);

/** Write to pin with given nr.
 *  Pin MUST be set to output mode, before.
 *  Use int parameter value true for high level, false for low level output.
 */
void gpio_write(int const pin_nr, int const high);

/** Read from pin with given nr.
 *  Works for pins set as input AND pins set as output, too.
 *  Returns true for high and false for low value read.
 */

#endif

