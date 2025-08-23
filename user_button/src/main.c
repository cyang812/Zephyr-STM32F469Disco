

/*
 * @Author: AIPC cy950812@gmail.com
 * @Date: 2025-08-16 22:25:06
 * @LastEditors: AIPC cy950812@gmail.com
 * @LastEditTime: 2025-08-18 21:05:47
 * @FilePath: _zephyr_stm32f469i_zephyr_samples_myapp_random_src_main.c
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

#include "say_hello.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)

#define USER_BUTTON_NODE DT_ALIAS(sw0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec user_button = GPIO_DT_SPEC_GET(USER_BUTTON_NODE, gpios);

int main(void)
{
	int ret;
	bool led0_state = true;
	bool user_button_state = false;

	if (!gpio_is_ready_dt(&led0)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	if (!gpio_is_ready_dt(&led1)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	int delay = 0;
	while (1) {

		if (delay%10 == 0) {
			bool button_state = gpio_pin_get_dt(&user_button);
			if (user_button_state != button_state) {
				user_button_state = button_state;
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
