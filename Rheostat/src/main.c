#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(rheo_app, LOG_LEVEL_INF);

/* point to zephyr_user.rheo in the overlay */
#define RHEO_NODE DT_NODELABEL(rheo)

#if !DT_NODE_HAS_STATUS(RHEO_NODE, okay)
#error "No rheostat ADC channel found in devicetree"
#endif

static const struct adc_dt_spec adc_ch = ADC_DT_SPEC_GET(RHEO_NODE);
static int16_t sample_buffer;

void main(void)
{
    int ret;

    if (!device_is_ready(adc_ch.dev)) {
        LOG_ERR("ADC device not ready");
        return;
    }

    ret = adc_channel_setup_dt(&adc_ch);
    if (ret < 0) {
        LOG_ERR("Channel setup failed (%d)", ret);
        return;
    }

    struct adc_sequence sequence = {
        .buffer      = &sample_buffer,
        .buffer_size = sizeof(sample_buffer),
        .resolution  = 12,
    };

    LOG_INF("Using channel %d, vref=%d mV", adc_ch.channel_id, adc_ch.vref_mv);

    while (1) {
        ret = adc_read(adc_ch.dev, &sequence);
        if (ret == 0) {
            int32_t mv = sample_buffer;
            adc_raw_to_millivolts_dt(&adc_ch, &mv);
            LOG_INF("Raw=%d → %d mV", sample_buffer, mv);
        } else {
            LOG_ERR("adc_read failed (%d)", ret);
        }

        k_sleep(K_MSEC(1000));
    }
}
