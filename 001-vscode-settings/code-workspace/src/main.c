/**
 * SPDX-License-Identifier: BSL-1.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>

typedef int (*my_device_do_something_t)(const struct device *dev, int val);
typedef int (*my_device_do_something_else_t)(const struct device *dev);

struct my_device_driver_api {
    my_device_do_something_t do_something;
    my_device_do_something_else_t do_something_else;
};

static inline int my_device_do_something(const struct device *dev, int val)
{
    const struct my_device_driver_api *api = dev->api;
    return api->do_something(dev, val);
}

static inline int my_device_do_something_else(const struct device *dev)
{
    const struct my_device_driver_api *api = dev->api;
    return api->do_something_else(dev);
}

int main(void) {
    int ret;
    const struct device *dev = device_get_binding("my_device0");
    if (!dev) {
        printk("Failed to get device\n");
        ret = -ENODEV;
        goto out;
    }

    if (!device_is_ready(dev)) {
        printk("Device is not ready\n");
        ret = -ENODEV;
        goto out;
    }
    
    while (true) {
        my_device_do_something(dev, 3);
        my_device_do_something_else(dev);
        k_sleep(K_MSEC(1000));
    }
 
out:
    return ret;
}

struct my_device_some_impl_data {
    int foo;
};

struct my_device_some_impl_config {
    int bar;
};

static int my_device_some_impl_do_something(const struct device *dev, int val)
{
    struct my_device_some_impl_data *data = dev->data;

    data->foo += val;

    printk("Did something! foo=%d\n", data->foo);
    return 0;
}

static int my_device_some_impl_do_something_else(const struct device *dev)
{
    struct my_device_some_impl_data *data = dev->data;
    const struct my_device_some_impl_config *config = dev->config;

    data->foo += config->bar;

    printk("Did something else! foo=%d\n", data->foo);
    return 0;
}

static int my_impl_init(const struct device *dev)
{
    printk("Device %s initialized!\n", dev->name);
    return 0;
}

static DEVICE_API(my_device, some_impl_api) = {
    .do_something = my_device_some_impl_do_something,
    .do_something_else = my_device_some_impl_do_something_else,
};

static struct my_device_some_impl_data my_impl_data0 = {
    .foo = 0,
};
static struct my_device_some_impl_config my_impl_config0 = {
    .bar = 2,
};

DEVICE_DEFINE(my_device_some_impl0, "my_device0", my_impl_init,
    my_impl_init, &my_impl_data0, &my_impl_config0,
    POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &some_impl_api);