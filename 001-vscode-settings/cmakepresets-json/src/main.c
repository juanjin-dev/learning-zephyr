/**
 * SPDX-License-Identifier: BSL-1.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/led.h>
struct pwmled_data {
    const struct led_dt_spec pwmled;
    bool dir;
    uint8_t brightness;
};

int main(void) {
    int ret;
    struct k_timer timer;
    struct pwmled_data pwmleds[] = {
        {
            .pwmled = LED_DT_SPEC_GET(DT_NODELABEL(pwmled0)),
            .dir = false,
            .brightness = 0,
        },
        {
            .pwmled = LED_DT_SPEC_GET(DT_NODELABEL(pwmled1)),
            .dir = true,
            .brightness = 100,
        }
    };

    ARRAY_FOR_EACH_PTR(pwmleds, pwmled) {
        if (!led_is_ready_dt(&pwmled->pwmled)) {
            printk("Error: LED %s is not ready\n",
                    pwmled->pwmled.dev->name);
            goto out;
        }
    }

    k_timer_init(&timer, NULL, NULL);
    k_timer_start(&timer, K_NO_WAIT, K_MSEC(5));

    while (true) {
        int elapsed = k_timer_status_sync(&timer);
        if (elapsed > 1) {
            printk("Error: timer is %d ms late\n", elapsed);
            k_timer_stop(&timer);
            goto out;
        }

        ARRAY_FOR_EACH_PTR(pwmleds, pwmled) {
            ret = led_set_brightness_dt(&pwmled->pwmled, pwmled->brightness);
            if (ret) {
                printk("Error %d: failed to set pulse width\n", ret);
                k_timer_stop(&timer);
                goto out;
            }

            if (!pwmled->dir) {
                pwmled->brightness += 1;
            } else {
                pwmled->brightness -= 1;
            }

            if (pwmled->brightness == 0 || pwmled->brightness == 100) {
                pwmled->dir = !pwmled->dir;
            }
        }
    }

out:
    return 0;
}