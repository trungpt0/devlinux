#include <linux/module.h>

/**
 * Module initialization function
*/
static int __init hello_world_driver_init(void)
{
    pr_info("Hello World!\n");
    pr_info("Kernel Module Inserted Successfully!\n");
    return 0;
}

/**
 * Module exit function
*/
static void __exit hello_world_driver_exit(void)
{
    pr_info("Kernel Module Removed Successfully!\n");
}

/**
 * Register initialization and exit functions of the module
*/
module_init(hello_world_driver_init);
module_exit(hello_world_driver_exit);

/**
 * Module Information
*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DEVLINUX - Trung Tran");
MODULE_DESCRIPTION("Hello World Kernel Module");
MODULE_VERSION("1.0");
