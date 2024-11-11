#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define BUFFER_SIZE 1024

dev_t dev_num = 0; // Device number
static struct class *dev_class = NULL; // Device class
static struct cdev mycdev_cdev; // Character device structure
static char *k_buffer = NULL;

// Function to handle opening the device
static int device_open(struct inode *inode, struct file *filp)
{
    pr_info("device was opened!\n");
    return 0;
}

// Function to handle closing the device
static int device_release(struct inode *inode, struct file *file)
{
    pr_info("device was closed!\n");
    return 0;
}

// Function to handle reading from the device
static ssize_t device_read(struct file *filp, char __user *buffer, size_t len, loff_t *off)
{
    if (*off >= BUFFER_SIZE) return 0;
    if (len > BUFFER_SIZE - *off) len = BUFFER_SIZE - *off;

    if (copy_to_user(buffer, k_buffer + *off, len)) {
        pr_err("cannot copy data to user\n");
        return -EFAULT;
    }

    *off += len;
    pr_info("device was read!\n");
    return len;
}

// Function to handle writing to the device
static ssize_t device_write(struct file *filp, const char __user *buffer, size_t len, loff_t *off)
{
    if (*off >= BUFFER_SIZE) return 0;
    if (len > BUFFER_SIZE - *off) len = BUFFER_SIZE - *off;

    if (copy_from_user(k_buffer + *off, buffer, len)) {
        pr_err("cannot copy data to kernel\n");
        return -EFAULT;
    }

    *off += len;
    pr_info("device was write!\n");
    return len;
}

static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
};

/**
 * Module initialization function
*/
static int __init char_dev_driver_init(void)
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

    /* Allocate kernel buffer */
    k_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!k_buffer) {
        pr_err("failed to allocate memory for kernel buffer\n");
        goto dev_err;
    }

    cdev_init(&devlinux_cdev, &fops);
    if (cdev_add(&devlinux_cdev, dev_num, 1) < 0) {
        pr_err("can not add character device\n");
        goto cdev_err;
    }

    pr_info("Kernel Module Inserted Successfully!\n");
    return 0;

cdev_err:
    kfree(k_buffer);
    device_destroy(dev_class, dev_num);
dev_err:
    class_destroy(dev_class);
class_err:
    unregister_chrdev_region(dev_num, 1);
    return -1;
}

/**
 * Module exit function
*/
static void __exit char_dev_driver_exit(void)
{
    kfree(k_buffer); // Free the kernel buffer
    cdev_del(&devlinux_cdev); // Remove the character device
    device_destroy(dev_class, dev_num); // Removes a device
    class_destroy(dev_class); // Destroys a struct class structure
    unregister_chrdev_region(dev_num, 1); // Unregister a range of device numbers
    pr_info("Kernel Module Removed Successfully!\n");
}

/**
 * Register initialization and exit functions of the module
*/
module_init(char_dev_driver_init);
module_exit(char_dev_driver_exit);

/**
 * Module Information
*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DEVLINUX - Trung Tran");
MODULE_DESCRIPTION("Interacting with device file");
MODULE_VERSION("1.0");
