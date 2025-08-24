

/*
 * @Author: AIPC cy950812@gmail.com
 * @Date: 2025-08-16 22:25:06
 * @LastEditors: AIPC cy950812@gmail.com
 * @LastEditTime: 2025-08-24 08:44:30
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
#include <zephyr/random/random.h>
#include <zephyr/drivers/sensor.h>

#include "say_hello.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   3000

/* The devicetree node identifier for the "led" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)

#define SLED0_NODE DT_ALIAS(sled0)
#define SLED1_NODE DT_ALIAS(sled1)
#define SLED2_NODE DT_ALIAS(sled2)

#define USER_BUTTON_NODE0 DT_ALIAS(sw0)
#define USER_BUTTON_NODE1 DT_ALIAS(sw1)
#define USER_BUTTON_NODE2 DT_ALIAS(sw2)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

static const struct gpio_dt_spec sled0 = GPIO_DT_SPEC_GET(SLED0_NODE, gpios);
static const struct gpio_dt_spec sled1 = GPIO_DT_SPEC_GET(SLED1_NODE, gpios);
static const struct gpio_dt_spec sled2 = GPIO_DT_SPEC_GET(SLED2_NODE, gpios);

static const struct gpio_dt_spec user_button0 = GPIO_DT_SPEC_GET(USER_BUTTON_NODE0, gpios);
static const struct gpio_dt_spec user_button1 = GPIO_DT_SPEC_GET(USER_BUTTON_NODE1, gpios);
static const struct gpio_dt_spec user_button2 = GPIO_DT_SPEC_GET(USER_BUTTON_NODE2, gpios);

static const struct device *dht11 = DEVICE_DT_GET(DT_ALIAS(dht11));

int main(void)
{
    int ret;
    bool led0_state = true;
    bool button0_state = false;
    bool button1_state = false;
    bool button2_state = false;

    if (!(gpio_is_ready_dt(&led0) &&
          gpio_is_ready_dt(&led1) &&
          gpio_is_ready_dt(&led2) &&
          gpio_is_ready_dt(&led3) &&
          gpio_is_ready_dt(&sled0) &&
          gpio_is_ready_dt(&sled1) &&
          gpio_is_ready_dt(&sled2) &&
          gpio_is_ready_dt(&user_button0) &&
          gpio_is_ready_dt(&user_button1) &&
          gpio_is_ready_dt(&user_button2))) {
        printk("Error: not all GPIOs are ready\n");
        return 0;
    }

    ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE) +
          gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE) +
          gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE) +
          gpio_pin_configure_dt(&led3, GPIO_OUTPUT_ACTIVE) +
          gpio_pin_configure_dt(&sled0, GPIO_OUTPUT_ACTIVE) +
          gpio_pin_configure_dt(&sled1, GPIO_OUTPUT_ACTIVE) +
          gpio_pin_configure_dt(&sled2, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED device %s\n",
               ret, led0.port->name);
        return 0;
    }

     if (!device_is_ready(dht11)) {
        printk("DHT11 device not ready\n");
        return;
    }

    int delay = 0;
    while (1) {

        if (delay%10 == 0) {
            bool btn0 = gpio_pin_get_dt(&user_button0);
            bool btn1 = gpio_pin_get_dt(&user_button1);
            bool btn2 = gpio_pin_get_dt(&user_button2);

            if (button0_state != btn0) {
                button0_state = btn0;
                gpio_pin_toggle_dt(&led1);
            }

            if (button1_state != btn1) {
                button1_state = btn1;
                gpio_pin_toggle_dt(&led2);
            }

            if (button2_state != btn2) {
                button2_state = btn2;
                gpio_pin_toggle_dt(&led3);
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

            if (sensor_sample_fetch(dht11) == 0) {
                struct sensor_value temp, hum;
                sensor_channel_get(dht11, SENSOR_CHAN_AMBIENT_TEMP, &temp);
                sensor_channel_get(dht11, SENSOR_CHAN_HUMIDITY, &hum);

                printk("Temp: %d.%06d °C, Hum: %d.%06d %%\n",
                    temp.val1, temp.val2,
                    hum.val1, hum.val2);
            } else {
                printk("DHT11 sample fetch failed\n");
            }
        }

        k_msleep(1);
        delay++;
    }
    return 0;
}
