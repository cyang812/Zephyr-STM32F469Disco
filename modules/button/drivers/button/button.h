/*
 * @Author: AIPC cy950812@gmail.com
 * @Date: 2025-08-19 20:10:47
 * @LastEditors: AIPC cy950812@gmail.com
 * @LastEditTime: 2025-08-23 10:27:14
 * @FilePath: _zephyr_stm32f469i_zephyr_samples_myapp_modules_button_drivers_button_button.h
 * @Description:
 *
 * Copyright (c) 2025 by AIPC, All Rights Reserved.
 */
#ifndef USER_BUTTON_H_
#define USER_BUTTON_H_

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

struct button_api {
    int (*get_press_state)(const struct device *dev, bool *state);
};

struct button_config {
    struct gpio_dt_spec button_gpio;
    int id;
};

#endif /* USER_BUTTON_H_ */
