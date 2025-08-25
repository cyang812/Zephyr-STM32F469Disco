

/*
 * @Author: AIPC cy950812@gmail.com
 * @Date: 2025-08-16 22:25:06
 * @LastEditors: AIPC cy950812@gmail.com
 * @LastEditTime: 2025-08-25 20:05:42
 * @FilePath: _myapp_DHT11_src_main.c
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
#include <zephyr/drivers/sensor.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   3000
#define LED0_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

static const struct device *dht11 = DEVICE_DT_GET(DT_ALIAS(dht11));

int main(void)
{
    int ret;

    if (!gpio_is_ready_dt(&led0)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

    if (!device_is_ready(dht11)) {
        printk("DHT11 device not ready\n");
        return;
    }

    k_msleep(SLEEP_TIME_MS);

    while (1) {

        gpio_pin_toggle_dt(&led0);

        ret = sensor_sample_fetch(dht11);
        if (ret == 0) {
            struct sensor_value temp, hum;
            sensor_channel_get(dht11, SENSOR_CHAN_AMBIENT_TEMP, &temp);
            sensor_channel_get(dht11, SENSOR_CHAN_HUMIDITY, &hum);

            printk("Temp: %d.%06d °C, Hum: %d.%06d %%\n",
                temp.val1, temp.val2,
                hum.val1, hum.val2);
        } else {
            printk("DHT11 sample fetch failed, %d\n", ret);
        }

        k_msleep(SLEEP_TIME_MS);
    }

    return 0;
}
