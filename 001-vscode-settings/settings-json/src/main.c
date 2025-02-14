/**
 * SPDX-License-Identifier: BSL-1.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

int main(void) {
    const struct gpio_dt_spec led =
        GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

    if (!device_is_ready(led.port)) {
        printk("Error: GPIO port %s is not ready\n", led.port->name);
        return -ENODEV;
    }

    printk("Hell Zephyr!: %s(%s, %s), %lldms\n",
        CONFIG_BOARD, CONFIG_ARCH, CONFIG_SOC, k_uptime_get());

    while (true) {
        gpio_pin_toggle_dt(&led);
        printk("Hell GPIO!: %d\n", gpio_pin_get_dt(&led));
        k_sleep(K_MSEC(1000));
    }

    return 0;
}