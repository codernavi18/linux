#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/device.h>
#include <linux/cdev.h>

static unsigned int major;
static struct class *dummy_class;
static struct cdev dummy_cdev;

int dummy_open(struct inode* inode, struct file* filep)
{
        pr_info("Someone tried opening the file\n");
        return 0;
}

int dummy_release(struct inode* inode, struct file* filep)
{
        pr_info("Someone closed me\n");
        return 0;
}

ssize_t dummy_read(struct file* filep, char __user* buf, size_t count, loff_t* offset)
{
        pr_info("nothing to read\n");
        return 0;
}

ssize_t dummy_write(struct file* filep, const char __user* buf, size_t count, loff_t* offset)
{
        pr_info("can'r accept any data\n");
        return count;
}

struct file_operations dummy_fops = {
        open: dummy_open,
        release: dummy_release,
        read: dummy_read,
        write: dummy_write
};

static int __init my_init(void)
{
        struct device* dummy_device;
        int error;
        dev_t devt = 0;

        error = alloc_chrdev_region(&devt,0,1,"dummy_char");
        if(error < 0)
        {
                pr_err("can't get major number\n");
                return error;
        }

        major = MAJOR(devt);
        pr_info("Major number : %d\n",major);

        dummy_class = class_create(THIS_MODULE,"dumy_char_class");
        if(IS_ERR(dummy_class))
        {
                pr_err("Could not get device class\n");
                unregister_chrdev_region(MKDEV(major,0),1);
                return PTR_ERR(dummy_class);
        }

        cdev_init(&dummy_cdev,&dummy_fops);
        dummy_cdev.owner = THIS_MODULE;

        cdev_add(&dummy_cdev,devt,1);

        dummy_device = device_create(dummy_class,NULL,devt,NULL,"dummy_char");

        if(IS_ERR(dummy_device))
        {
                pr_err("error creating device\n");
                cdev_del(&dummy_cdev);
                class_destroy(dummy_class);
                unregister_chrdev_region(devt,1);
                return -1;
        }

        pr_info("dummy_char module loaded\n");
        return 0;
}

static void __exit my_exit(void)
{
        device_destroy(dummy_class,MKDEV(major,0));
        cdev_del(&dummy_cdev);
        class_destroy(dummy_class);
        unregister_chrdev_region(MKDEV(major,0),1);
        pr_info("dummy_char module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("naveen");
MODULE_DESCRIPTION("Dummy character driver");
MODULE_LICENSE("GPL");
