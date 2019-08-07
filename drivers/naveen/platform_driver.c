#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>

static int my_pdrv_probe(struct platform_device *pdev)
{
	pr_info("platform device probe called\n");
	return 0;
}

static int my_pdrv_remove(struct platform_device *pdev)
{
	pr_info("platform driver remove called\n");
	return 0;
}

static struct platform_driver mypdrv =
{
	.probe = my_pdrv_probe,
	.remove = my_pdrv_remove,
	.driver = {
		.name = "MY_PDRV",
		.owner = THIS_MODULE,
	},
};

module_platform_driver(mypdrv);

MODULE_AUTHOR("naveen");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("dummy platform driver");
