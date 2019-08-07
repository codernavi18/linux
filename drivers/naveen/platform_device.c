#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/types.h>

static struct platform_device *pdev;

static int __init platform_device_insert(void)
{
	pdev = platform_device_alloc("MY_PDRV",0);
	platform_device_add(pdev);
	pr_info("platform device added\n");
	return 0;
}

static void __exit platform_device_remove(void)
{
	platform_device_put(pdev);
	pr_info("MY_PDRV device removed\n");
}

module_init(platform_device_insert);
module_exit(platform_device_remove);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("naveen");
