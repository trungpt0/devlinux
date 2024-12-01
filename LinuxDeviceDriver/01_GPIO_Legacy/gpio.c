#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/uaccess.h>

#define BCM2835_GPIO_BASE  0x20200000  
#define GPIO_SET_OFFSET    0x1C        
#define GPIO_CLR_OFFSET    0x28        
#define GPIO_FSEL_OFFSET   0x00        
#define GPIO_PIN           27          

static void __iomem *gpio_base;

static int __init gpio_driver_init(void)
{
    uint32_t reg;

    gpio_base = ioremap(BCM2835_GPIO_BASE, 0x100);
    if (!gpio_base) {
        pr_err("Failed to ioremap GPIO\n");
        return -ENOMEM;
    }

    reg = ioread32(gpio_base + GPIO_FSEL_OFFSET + (GPIO_PIN / 10) * 4);
    reg &= ~(7 << ((GPIO_PIN % 10) * 3)); 
    reg |= (1 << ((GPIO_PIN % 10) * 3));  
    iowrite32(reg, gpio_base + GPIO_FSEL_OFFSET + (GPIO_PIN / 10) * 4);

    iowrite32(1 << GPIO_PIN, gpio_base + GPIO_SET_OFFSET);
    pr_info("GPIO27 set to HIGH\n");

    return 0;
}

static void __exit gpio_driver_exit(void)
{
    iowrite32(1 << GPIO_PIN, gpio_base + GPIO_CLR_OFFSET);
    pr_info("GPIO27 set to LOW\n");

    if (gpio_base)
        iounmap(gpio_base);
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Trung Tran");
MODULE_DESCRIPTION("Simple GPIO Driver for Raspberry Pi Zero W");

