#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

dev_t dev_num = 0; // Device number

/**
 * Module initialization function
*/
static int __init device_number_driver_init(void)
{
    int ret;

    /* Register a range of device numbers. The major number will be chosen dynamically. */
    if ((ret = alloc_chrdev_region(&dev_num, 0, 1, "dev_number")) != 0) {
        pr_err("can not alloc device major\n");
        return -1;
    }
    pr_info("Major = %d Minor = %d\n", MAJOR(dev_num), MINOR(dev_num));

    pr_info("Kernel Module Inserted Successfully!\n");
    return 0;
}

/**
 * Module exit function
*/
static void __exit device_number_driver_exit(void)
{
    unregister_chrdev_region(dev_num, 1); // Unregister a range of device numbers
    pr_info("Kernel Module Removed Successfully!\n");
}

/**
 * Register initialization and exit functions of the module
*/
module_init(device_number_driver_init);
module_exit(device_number_driver_exit);

/**
 * Module Information
*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DEVLINUX - Trung Tran");
MODULE_DESCRIPTION("Device Number Kernel Module");
MODULE_VERSION("1.0");
