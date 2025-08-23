#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <say_hello.h>
#include "button.h"

/*
 * Get the node identifiers for the user buttons that are actually defined.
 */
#define USER_BUTTON_NODE2 DT_ALIAS(user_sw2)
#define USER_BUTTON_NODE3 DT_ALIAS(user_sw3)

/*
 * A build-time check that the aliases exist in the devicetree.
 */
#if !DT_NODE_HAS_STATUS(USER_BUTTON_NODE2, okay)
#error "Unsupported board: user_sw2 devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(USER_BUTTON_NODE3, okay)
#error "Unsupported board: user_sw3 devicetree alias is not defined"
#endif

/*
 * Get the device pointers for the buttons.
 */
static const struct device *user_button2 = DEVICE_DT_GET(USER_BUTTON_NODE2);
static const struct device *user_button3 = DEVICE_DT_GET(USER_BUTTON_NODE3);

void main(void)
{
	printk("Hello World! %s\n", CONFIG_BOARD);
	say_hello();

	if (!device_is_ready(user_button2) || !device_is_ready(user_button3)) {
		printk("Error: One or more custom button devices are not ready\n");
		return;
	}

	printk("Custom button devices are ready. Polling...\n");

	while(1) {
		/* Get the API from the device struct for button 2 */
		const struct button_api *api2 = user_button2->api;
		if (api2 && api2->get_press_state) {
			if (api2->get_press_state(user_button2) > 0) {
				printk("Button 2 is pressed!\n");
			}
		}

		/* Get the API from the device struct for button 3 */
		const struct button_api *api3 = user_button3->api;
		if (api3 && api3->get_press_state) {
			if (api3->get_press_state(user_button3) > 0) {
				printk("Button 3 is pressed!\n");
			}
		}

		k_msleep(200);
	}
}
