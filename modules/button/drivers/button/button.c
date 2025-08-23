

/*
 * @Author: AIPC cy950812@gmail.com
 * @Date: 2025-08-19 21:45:54
 * @LastEditors: AIPC cy950812@gmail.com
 * @LastEditTime: 2025-08-23 22:12:00
 * @FilePath: _myapp_modules_button_drivers_button_button.c
 * @Description:
 *
 * Copyright (c) 2025 by AIPC, All Rights Reserved.
 */
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/devicetree.h>
#include "button.h"

struct custom_button_config {
    /* The driver now directly gets the full GPIO spec */
    struct gpio_dt_spec button_gpio;
};

static int custom_button_get_state(const struct device *dev)
{
    const struct custom_button_config *config = dev->config;
    return gpio_pin_get_dt(&config->button_gpio);
}

const struct custom_button_api custom_button_driver_api = {
    .get_press_state = custom_button_get_state,
};

static int custom_button_init(const struct device *dev)
{
    const struct custom_button_config *config = dev->config;
    int ret;

    if (!gpio_is_ready_dt(&config->button_gpio)) {
        printk("Error: button GPIO port %s is not ready\n", config->button_gpio.port->name);
        return -ENODEV;
    }

    ret = gpio_pin_configure_dt(&config->button_gpio, GPIO_INPUT);
    if (ret != 0) {
        printk("Error %d: failed to configure pin %d\n", ret, config->button_gpio.pin);
        return ret;
    }

    printk("Custom button driver initialized for %s on pin %d\n", dev->name, config->button_gpio.pin);
    return 0;
}

/* --- This instantiation part is now much simpler --- */
#define CREATE_CUSTOM_BUTTON(inst)                                                                  \
    const static struct custom_button_config custom_button_config_##inst = {                        \
        /* Directly get the 'gpios' property from the instance */                                   \
        .button_gpio = GPIO_DT_SPEC_INST_GET(inst, gpios),                                          \
    };                                                                                              \
                                                                                                    \
    DEVICE_DT_INST_DEFINE(inst,                                                                     \
                          &custom_button_init,                                                      \
                          NULL,                                                                     \
                          NULL,                                                                     \
                          &custom_button_config_##inst,                                             \
                          POST_KERNEL,                                                              \
                          CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                                      \
                          &custom_button_driver_api);

DT_INST_FOREACH_STATUS_OKAY(CREATE_CUSTOM_BUTTON)
