#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(Rheostat);

#define ADC_RESOLUTION 12
#define ADC_MAX 4095
#define ADC_CHANNEL 9      // PB1 = ADC1_IN9
#define VREF_MV 3300        // STM32 VCC in mV

void main(void)
{
    const struct device *adc_dev = DEVICE_DT_GET(DT_NODELABEL(adc1));
    if (!device_is_ready(adc_dev)) {
        LOG_ERR("ADC device not ready!");
        return;
    }

    int16_t sample;

    const struct adc_channel_cfg channel_cfg = {
        .gain = ADC_GAIN_1,
        .reference = ADC_REF_INTERNAL,
        .acquisition_time = ADC_ACQ_TIME_DEFAULT,
        .channel_id = ADC_CHANNEL,
        .differential = 0,
    };

    adc_channel_setup(adc_dev, &channel_cfg);

    while (1) {
        struct adc_sequence seq = {
            .channels = BIT(ADC_CHANNEL),
            .buffer = &sample,
            .buffer_size = sizeof(sample),
            .resolution = ADC_RESOLUTION,
        };

        if (adc_read(adc_dev, &seq) == 0) {
            LOG_INF("Sample=%d", sample);
        } else {
            LOG_WRN("ADC read failed");
        }

        k_sleep(K_SECONDS(1));
    }
}
