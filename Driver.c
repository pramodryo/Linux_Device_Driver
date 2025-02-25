#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/err.h>
#include <linux/export.h>
#include <linux/ioctl.h>


#define WR_VALUE _IOW('a','b',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)

#define mem_size   1024
dev_t dev=0;
int32_t   value = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
uint8_t *kernel_buffer;

static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);

static int etx_open(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);
static int etx_release(struct inode *inode, struct file *file);
static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg);



static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .read = etx_read,
    .open = etx_open,
    .write = etx_write,
    .unlocked_ioctl = etx_ioctl,
    .release = etx_release,
};

static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd)
    {

        case WR_VALUE:
            if(copy_from_user(&value, (int32_t *)arg, sizeof(value)))
            {
                pr_err("Data Write : Error!\n");
            }
            pr_info("Value: %d\n", value);
            break;

        case RD_VALUE:
            if(copy_to_user((int32_t *)arg, &value, sizeof(value)))
            {
                pr_err("Data Read: Err! \n");
            }
            break;

        default:
            pr_info("ioctl Default\n");
    }
return 0;
}//etx_ioctl

static int etx_open(struct inode *inode, struct file *file)
{
    pr_info("Device file open...!!!\n");
    return 0;
}//etx_open

static int etx_release(struct inode *inode, struct file *file)
{
    pr_info("Device file closed....!!!!\n");
    return 0;
}//etx_release

static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    //Copy the data from the kernel space to the user-space
    if( copy_to_user(buf, kernel_buffer, mem_size) )
    {
        pr_err("Data Read : Err!\n");
    }
    pr_info("Data Read : Done!\n");
    return mem_size;
}//etx_read

static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    if(copy_from_user(kernel_buffer, buf, len))
    {
        pr_err("Data write: ERROR\n");
    }
    pr_info("Data Write Done...\n");
    return len;
}//etx_write


static int __init etx_driver_init(void)
{
    if((alloc_chrdev_region(&dev, 0,1, "etx_dev"))<0)
    {
        pr_info("cannot allocate major number\n");
        return -1;
    }
    pr_info("major:%d minor:%d\n", MAJOR(dev), MINOR(dev));
    cdev_init(&etx_cdev, &fops);

    if((cdev_add(&etx_cdev, dev,1)) <0)
    {
        pr_info("cannot add the device to the system\n");
        goto r_class;
    }

    if(IS_ERR(dev_class = class_create("etx_class")))
    {
        pr_info("cannot create the struct class..\n");
        goto r_class;
    }

    if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"etx_device")))
    {
        pr_info("cannot create the device 1\n");
        goto r_device;
    }

    if((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0)
    {
        pr_info("cannot allocate memory in kernel \n");
        goto r_device;
    }

    strcpy(kernel_buffer, "hello_world");
    pr_info("Device driver insert done...\n");
    return 0;


r_device:
    class_destroy(dev_class);

r_class:
    unregister_chrdev_region(dev,1);
    return -1;

}//etx_driver_init

static void __exit etx_driver_exit(void)
{
    kfree(kernel_buffer);
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("device Driver Remove....Done\n");
}//etx_driver_exit


module_init(etx_driver_init);
module_exit(etx_driver_exit);


//MODULE_LICENCE("GPL");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Pramod Rathod");
MODULE_DESCRIPTION("simple linux DD");
MODULE_VERSION("2:1.0");



