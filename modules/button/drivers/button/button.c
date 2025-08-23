// Ties to the 'compatible = "custome,button"' node in the devicetree
#define DT_DRV_COMPAT custome_button

#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "button.h"

// Enable logging for this module
LOG_MODULE_REGISTER(button);

//-------------------------------------------------------------------
// Forward declaration of the button API structure

static int button_init(const struct device *dev);
static int button_get_press_state(const struct device *dev, bool *state);

//-------------------------------------------------------------------
// Private functions

static int button_init(const struct device *dev)
{
    int ret;

    const struct button_config *config = dev->config;
    const struct gpio_dt_spec *button = &config->button_gpio;

    LOG_DBG("Initializing button with ID: %d", config->id);
    if (!gpio_is_ready_dt(button)) {
        LOG_ERR("Button GPIO is not ready");
        return -ENODEV;
    }

    ret = gpio_pin_configure_dt(button, GPIO_INPUT);
    if (ret < 0) {
        LOG_ERR("Failed to configure button GPIO: %d", ret);
        return ret;
    }

    LOG_INF("Button initialized successfully");
    return 0;
}

//-------------------------------------------------------------------
// Public functions

static int button_get_press_state(const struct device *dev, bool *state)
{
    const struct button_config *config = dev->config;
    const struct gpio_dt_spec *button = &config->button_gpio;

    int ret = gpio_pin_get_dt(button);
    if (ret < 0) {
        LOG_ERR("Failed to read button state: %d", ret);
        return ret;
    }

    *state = (ret == 1);
    LOG_DBG("Button state: %s", (*state) ? "Pressed" : "Released");
    return 0;
}

//-------------------------------------------------------------------
// Devicetree handling

// Define the public API functions for the driver
const struct button_api button_api_funcs = {
    .get_press_state = button_get_press_state,
};

// Expansion macro to define driver instances
#define BUTTON_DEFINE(inst)                                                     \
                                                                                \
    /* Create an instance of the config struct, populate with DT values */      \
    static const struct button_config button_config_##inst = {                  \
        .button_gpio = GPIO_DT_SPEC_GET(                                        \
            DT_PHANDLE(DT_INST(inst, custome_button), pin), gpios),             \
        .id = inst                                                             \
    };                                                                          \
                                                                                \
    /* Create a "device" instance from a Devicetree node identifier and */      \
    /* registers the init function to run during boot. */                       \
    DEVICE_DT_INST_DEFINE(inst, button_init, NULL, NULL, &button_config_##inst, \
                          POST_KERNEL, CONFIG_GPIO_INIT_PRIORITY,               \
                          &button_api_funcs);                                   \

// The DeviceTree build process calls this to create an instanace of structs for
// each device (button) defined in the devicetree.
DT_INST_FOREACH_STATUS_OKAY(BUTTON_DEFINE)