#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>

#define LOW     0
#define HIGH    1

static struct gpio_desc *led_gpio_27;

static int my_pdrv_probe(struct platform_device *pdev)
{
        struct device *dev = &pdev->dev;

        /* init led GPIO with LOW */
        led_gpio_27 = gpiod_get(dev, "led", GPIOD_OUT_LOW);
        if (IS_ERR(led_gpio_27)) {
                pr_err("cannot init led gpio\n");
                return PTR_ERR(led_gpio_27);
        }

        gpiod_set_value(led_gpio_27, HIGH);

        pr_info("%s - %d", __func__, __LINE__);
        return 0;
}

static int my_pdrv_remove(struct platform_device *pdev)
{
        gpiod_set_value(led_gpio_27, LOW);
        gpiod_put(led_gpio_27);

        pr_info("%s - %d", __func__, __LINE__);
        return 0;
}

/* device tree match table */
static const struct of_device_id gpiod_dt_ids[] = {
        { .compatible = "gpio-descriptor-based", },
        { }
};

/* platform driver structure */
static struct platform_driver mypdrv = {
        .probe = my_pdrv_probe,
        .remove = my_pdrv_remove,
        .driver = {
                .name = "descriptor-based",
                .of_match_table = of_match_ptr(gpiod_dt_ids),
                .owner = THIS_MODULE,
        },
};
module_platform_driver(mypdrv);

/**
 * Module Information
*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DEVLINUX - Trung Tran");
MODULE_DESCRIPTION("Descriptor-based GPIO Driver");
MODULE_VERSION("1.0");
