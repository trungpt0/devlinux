/**
  * file: itg-gpio.c
  */
#include <linux/module.h>
#include <linux/gpio.h>

#define GPIO_NUMBER 539

#define LOW     0
#define HIGH    1

/**
 * Module initialization function
*/
static int __init integer_based_driver_init(void)
{
    int ret;

    ret = gpio_request(GPIO_NUMBER, "integer_gpio");

    if (ret) {
        pr_err("cannot to request GPIO %d\n", GPIO_NUMBER);
        return ret;
    }

    gpio_direction_output(GPIO_NUMBER, LOW);
    gpio_set_value(GPIO_NUMBER, HIGH);

    pr_info("Driver added successfully!\n");
    return 0;
}

/**
 * Module exit function
*/
static void __exit integer_based_driver_exit(void)
{
    gpio_set_value(GPIO_NUMBER, LOW);
    gpio_free(GPIO_NUMBER);
    pr_info("Driver added removed!\n");
}

/**
 * Register initialization and exit functions of the module
*/
module_init(integer_based_driver_init);
module_exit(integer_based_driver_exit);

/**
 * Module Information
*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DEVLINUX - Trung Tran");
MODULE_DESCRIPTION("Integer-based GPIO Driver");
MODULE_VERSION("1.0");
