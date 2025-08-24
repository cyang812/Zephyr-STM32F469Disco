#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/sys/printk.h>

/*
 * Get a device structure from a devicetree node with alias "potentiometer".
 * This is the modern and recommended way to reference devices.
 */
static const struct adc_dt_spec pot_spec = ADC_DT_SPEC_GET(DT_ALIAS(potentiometer));

// Buffer for the ADC reading
static int16_t sample_buffer;

/*
 * Define the ADC sequence for a single channel read.
 * This pulls configuration (resolution, etc.) from the device tree.
 */
static const struct adc_sequence sequence = {
    .buffer = &sample_buffer,
    .buffer_size = sizeof(sample_buffer), // Size of the buffer in bytes
};

void main(void)
{
    int err;
    int32_t val_mv;

    printk("Zephyr ADC Reader Example 🚀\n");
    printk("Board: %s\n", CONFIG_BOARD);

    // 1. Check if the ADC device is ready
    if (!device_is_ready(pot_spec.dev)) {
        printk("ADC controller device not ready\n");
        return;
    }

    // 2. Configure the ADC channel from the device tree spec
    err = adc_channel_setup_dt(&pot_spec);
    if (err < 0) {
        printk("Could not setup channel #%d (err %d)\n", pot_spec.channel_id, err);
        return;
    }

    while (1) {
        // 3. Perform the ADC read
        err = adc_read_dt(&pot_spec, &sequence);
        if (err < 0) {
            printk("Could not read ADC channel (%d)\n", err);
            continue;
        }

        // 4. Convert the raw ADC value to millivolts
        err = adc_raw_to_millivolts_dt(&pot_spec, &sample_buffer);
        if (err < 0) {
            printk("Could not convert to millivolts (%d)\n", err);
            continue;
        }
        val_mv = sample_buffer; // After conversion, buffer holds millivolts

        // 5. Print the result
        printk("ADC reading: %d mV\n", val_mv);

        // Wait before the next reading
        k_msleep(1000); // Sleep for 1 second
    }
}