

/*
 * @Author: AIPC cy950812@gmail.com
 * @Date: 2025-08-16 22:25:06
 * @LastEditors: AIPC cy950812@gmail.com
 * @LastEditTime: 2025-08-23 11:53:57
 * @FilePath: _zephyr_stm32f469i_zephyr_samples_myapp_custome_button_src_main.c
 * @Description:
 *
 * Copyright (c) 2025 by AIPC, All Rights Reserved.
 */
/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/random/random.h>
#include <zephyr/logging/log.h>

#include "say_hello.h"
#include "button.h"

LOG_MODULE_REGISTER(main);

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)

#define USER_BUTTON_NODE1 DT_ALIAS(sw0)
#define USER_BUTTON_NODE2 DT_ALIAS(user_sw1)
#define USER_BUTTON_NODE3 DT_ALIAS(user_sw2)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec user_button1 = GPIO_DT_SPEC_GET(USER_BUTTON_NODE1, gpios);

static const struct device *user_button2 = DEVICE_DT_GET(USER_BUTTON_NODE2);
static const struct device *user_button3 = DEVICE_DT_GET(USER_BUTTON_NODE3);

int main(void)
{
	int ret;
	bool led0_state = true;
	bool btn_state = false;

	// LED initialize
	if (!(gpio_is_ready_dt(&led0) && gpio_is_ready_dt(&led1))) {
		LOG_ERR("LED device not ready");
		return 0;
	}

	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		LOG_ERR("Failed to configure LED0 pin");
		return 0;
	}

	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		LOG_ERR("Failed to configure LED1 pin");
		return 0;
	}

	// button initialize
	if (!gpio_is_ready_dt(&user_button1)) {
		LOG_ERR("User button 1 device not ready");
		return 0;
	}

	// if (!(device_is_ready(user_button2) &&
	//       device_is_ready(user_button3))) {
	// 	return 0;
	// }

	// Get the API form one of the buttons
	const struct button_api *button_api1 = (const struct button_api *)user_button2->api;
	const struct button_api *button_api2 = (const struct button_api *)user_button3->api;

	int delay = 0;
	bool btn1_state = false;
	bool btn2_state = false;
	bool btn3_state = false;
	while (1) {

		btn1_state = gpio_pin_get_dt(&user_button1);

		ret = button_api1->get_press_state(user_button2, &btn2_state);
		if (ret < 0) {
			LOG_ERR("Failed to get button state: %d", ret);
			return 0;
		}

		// ret = button_api2->get_press_state(user_button3, &btn3_state);
		// if (ret < 0) {
		// 	LOG_ERR("Failed to get button state: %d", ret);
		// 	return 0;
		// }

		if (delay%10 == 0) {
			bool new_btn_state = btn1_state | btn2_state | btn3_state;
			if (btn_state != new_btn_state) {
				btn_state = new_btn_state;
				gpio_pin_toggle_dt(&led1);
			}
		}

		if (delay > SLEEP_TIME_MS) {
			delay = 0;
			ret = gpio_pin_toggle_dt(&led0);
			if (ret < 0) {
				return 0;
			}

			led0_state = !led0_state;
			printf("LED state: %s\n", led0_state ? "ON" : "OFF");

			uint32_t random_value = sys_rand32_get();
			printf("Random value: %u\n", random_value);

			double random_double = ((double)random_value / UINT32_MAX + 1.0);
			printf("Random double: %f\n", random_double);

			say_hello();
		}

		k_msleep(1);
		delay++;
	}
	return 0;
}
