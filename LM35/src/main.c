#include <zephyr/drivers/adc.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(lm35_app, LOG_LEVEL_INF);

/* Match the label from overlay */
#define ADC_CHANNEL_NODE DT_NODELABEL(lm35)

#if !DT_NODE_HAS_STATUS(ADC_CHANNEL_NODE, okay)
#error "No ADC channel for LM35 in devicetree"
#endif

static const struct adc_dt_spec adc_ch = ADC_DT_SPEC_GET(ADC_CHANNEL_NODE);

static int16_t sample_buffer;

void main(void)
{
    if (!device_is_ready(adc_ch.dev)) {
        LOG_ERR("ADC device not ready");
        return;
    }

    if (adc_channel_setup_dt(&adc_ch) < 0) {
        LOG_ERR("Channel setup failed");
        return;
    }

    struct adc_sequence sequence = {
        .buffer      = &sample_buffer,
        .buffer_size = sizeof(sample_buffer),
    };

    /* Let Zephyr fill in resolution + channel mask from devicetree */
    adc_sequence_init_dt(&adc_ch, &sequence);

    while (1) {
        int err = adc_read(adc_ch.dev, &sequence);
        if (err < 0) {
            LOG_ERR("adc_read failed (%d)", err);
        } else {
            LOG_INF("Raw=%d", sample_buffer);

            int32_t mv = sample_buffer;
            adc_raw_to_millivolts_dt(&adc_ch, &mv);

            float temp_c = mv / 10.0f;  /* LM35: 10 mV / °C */
            LOG_INF("LM35 temp=%.2f °C", temp_c);
        }

        k_sleep(K_MSEC(1000));
    }
}
