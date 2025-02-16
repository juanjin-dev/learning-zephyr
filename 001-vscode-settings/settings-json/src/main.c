/**
 * SPDX-License-Identifier: BSL-1.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/led.h>
#include <zephyr/input/input.h>

static void input_callback(struct input_event *evt, void *user_data)
{
    UNUSED(user_data);
    
    switch (evt->type) {
        case INPUT_EV_KEY: {
            switch (evt->code) {
                case INPUT_KEY_0: {
                    const struct led_dt_spec led =
                        LED_DT_SPEC_GET(DT_ALIAS(led0));
                    if (evt->value) {
                        led_on_dt(&led);
                    } else {
                        led_off_dt(&led);
                    }
                    break;
                }
                default: {
                    printk("Unknown key code: %u\n", evt->code);
                    break;
                }
            }
            break;
        }
        default: {
            printk("Unknown key type: %u\n", evt->type);
            break;
        }
    }
}

INPUT_CALLBACK_DEFINE(
    // DEVICE_DT_GET(DT_ALIAS(sw0)),
    NULL,
    input_callback, NULL
);

int main(void) {
    printk("Hell Zephyr!: %s(%s, %s), %lldms\r\n",
        CONFIG_BOARD, CONFIG_ARCH, CONFIG_SOC, k_uptime_get());

    return 0;
}