#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

dev_t dev_num = 0; // Device number
static struct class *dev_class = NULL; // Device class

/**
 * Module initialization function
*/
static int __init device_file_driver_init(void)
{
    int ret;

    /* Register a range of device numbers. The major number will be chosen dynamically. */
    if ((ret = alloc_chrdev_region(&dev_num, 0, 1, "dev_number")) != 0) {
        pr_err("can not alloc device major\n");
        return -1;
    }
    pr_info("Major = %d Minor = %d\n", MAJOR(dev_num), MINOR(dev_num));

    /* Create a struct class structure */
    dev_class = class_create(THIS_MODULE, "my_class");
    if (IS_ERR(dev_class)) {
        pr_err("can not create class device\n");
        goto class_err;
    }

    /* Creates a device */
    if (IS_ERR(device_create(dev_class, NULL, dev_num, NULL, "my_device"))) {
        pr_err("can not create device\n");
        goto dev_err;
    }

    pr_info("Kernel Module Inserted Successfully!\n");
    return 0;

dev_err:
    class_destroy(dev_class);
class_err:
    unregister_chrdev_region(dev_num, 1);
    return -1;
}

/**
 * Module exit function
*/
static void __exit device_file_driver_exit(void)
{
    device_destroy(dev_class, dev_num); // Removes a device
    class_destroy(dev_class); // Destroys a struct class structure
    unregister_chrdev_region(dev_num, 1); // Unregister a range of device numbers
    pr_info("Kernel Module Removed Successfully!\n");
}

/**
 * Register initialization and exit functions of the module
*/
module_init(device_file_driver_init);
module_exit(device_file_driver_exit);

/**
 * Module Information
*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DEVLINUX - Trung Tran");
MODULE_DESCRIPTION("Device File Driver");
MODULE_VERSION("1.0");
