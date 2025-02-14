/**
 * SPDX-License-Identifier: BSL-1.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

static void switch_callback(const struct device *port,
                            struct gpio_callback *cb, uint32_t pins)
{
    printk("GPIO interrupt!: %d\n", gpio_pin_get(port, pins));
}

int main(void) {
    int ret;
    const struct gpio_dt_spec switchy =
        GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
    struct gpio_callback switch_cb;

    if (!device_is_ready(switchy.port)) {
        printk("Error: GPIO port %s is not ready\n", switchy.port->name);
        goto out;
    }

    printk("Hell Zephyr!: %s(%s, %s), %lldms\r\n",
        CONFIG_BOARD, CONFIG_ARCH, CONFIG_SOC, k_uptime_get());

    ret = gpio_pin_configure_dt(&switchy, GPIO_INPUT);
    if (ret) {
        printk("Error: gpio_pin_configure_dt - %d\r\n", ret);
        goto out;
    }

    ret = gpio_pin_interrupt_configure_dt(&switchy, GPIO_INT_EDGE_BOTH);
    if (ret) {
        printk("Error: gpio_pin_interrupt_configure_dt - %d\r\n", ret);
        goto out;
    }

    gpio_init_callback(&switch_cb, switch_callback, BIT(switchy.pin));

    ret = gpio_add_callback_dt(&switchy, &switch_cb);
    if (ret) {
        printk("Error: gpio_add_callback_dt - %d\r\n", ret);
    }

    k_sleep(K_FOREVER);

out:
    return 0;
}