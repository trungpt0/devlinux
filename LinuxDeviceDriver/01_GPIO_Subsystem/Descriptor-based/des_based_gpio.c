#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/interrupt.h>

#define LOW     0
#define HIGH    1

static struct gpio_desc *led_gpio; // led descriptor
static struct gpio_desc *button_gpio; // button descriptor
static int irq_number;
static bool led_state = false;

/* handle interrupt function */
static irqreturn_t button_irq_handler(int irq, void *dev_id)
{
    led_state = !led_state;
    gpiod_set_value(led_gpio, led_state);
    pr_info("interrupted! led: %s\n", led_state ? "on" : "off");
    return IRQ_HANDLED;
}

/* probe driver */
static int des_gpio_probe(struct platform_device *pdev)
{
    int ret;
    struct device *dev = &pdev->dev;

    /* init led GPIO with LOW */
    led_gpio = gpiod_get(dev, "led", GPIOD_OUT_LOW);
    if (IS_ERR(led_gpio)) {
        pr_err("cannot init led gpio\n");
        return PTR_ERR(led_gpio);
    }

    /* init button GPIO with input mode*/
    button_gpio = gpiod_get(dev, "button", GPIOD_IN);
    if (IS_ERR(button_gpio)) {
        pr_err("cannot init button gpio\n");
        gpiod_put(led_gpio);
        return PTR_ERR(button_gpio);
    }

    /* get IRQ number */
    irq_number = gpiod_to_irq(button_gpio);
    if (irq_number < 0) {
        pr_err("cannot get irq number\n");
        gpiod_put(led_gpio);
        gpiod_put(button_gpio);
        return irq_number;
    }

    /* register irq number */
    ret = request_irq(irq_number, button_irq_handler, IRQF_TRIGGER_RISING, "button_irq", NULL);
    if (ret) {
        pr_err("cannot request irq\n");
        gpiod_put(led_gpio);
        gpiod_put(button_gpio);
        return ret;
    }

    pr_info("Driver added successfully!\n");
    return 0;
}

/* remove driver */
static int des_gpio_remove(struct platform_device *pdev)
{
    free_irq(irq_number, NULL);
    gpiod_set_value(led_gpio, LOW);
    gpiod_put(led_gpio);
    gpiod_put(button_gpio);
    pr_info("Driver added removed!\n");
    return 0;
}

/* device tree match table */
static const struct of_device_id gpiod_dt_ids[] = {
    { .compatible = "gpio-descriptor-based", },
    { }
};

/* platform driver structure */
static struct platform_driver mypdrv = {
    .probe = des_gpio_probe,
    .remove = des_gpio_remove,
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