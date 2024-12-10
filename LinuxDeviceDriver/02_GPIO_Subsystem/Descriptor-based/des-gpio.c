#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>

#define LOW     0
#define HIGH    1

struct gpio_desc *gpio_27;

static const struct of_device_id gpiod_dt_ids[] = {
    { .compatible = "gpio-descriptor-based", },
    { /* sentinel */ }
};

static int my_pdrv_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    gpio_27 = gpiod_get(dev, "led27", GPIOD_OUT_LOW);
    gpiod_set_value(gpio_27, HIGH);

    pr_info("%s - %d", __func__, __LINE__);
    return 0;
}

static int my_pdrv_remove(struct platform_device *pdev)
{
    gpiod_set_value(gpio_27, LOW);
    gpiod_put(gpio_27);

    pr_info("%s - %d", __func__, __LINE__);
    return 0;
}

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

MODULE_LICENSE("GPL");
MODULE_AUTHOR("trainee");
MODULE_DESCRIPTION("Descriptor-based GPIO");
MODULE_VERSION("1.0");
