#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/types.h>

struct my_gpios
{
	int reset_gpio;
	int led_gpio;
};

struct my_gpios needed_gpios = {
	.reset_gpio = 47,
	.led_gpio = 41,
};

static struct resource needed_resources[] = {
	[0] = {
		.start = 100,
		.end = 200,
		.flags = IORESOURCE_MEM,
		.name = "mem1",
	},
	[1] = {
		.start = 400,
		.end = 500,
		.flags = IORESOURCE_MEM,
		.name = "mem2",
	},
	[2] = {
		.start = 999,
		.end = 999,
		.flags = IORESOURCE_IRQ,
		.name = "mc",
	},
};

void my_dev_release(struct device *dev)
{
	pr_info("my device release called\n");
}

static struct platform_device my_pd = {
	.name = "MY_PDRV",
	.id = 0,
	.dev = {
		.platform_data = &needed_gpios,
		.release = my_dev_release,
	},
	.resource = needed_resources,
	.num_resources = ARRAY_SIZE(needed_resources),
};

static int __init my_init(void)
{
	pr_info("platform device module loaded, registering device\n");
	platform_device_register(&my_pd);
	return 0;
}

static void __exit my_exit(void)
{
	pr_info("platform device module unloaded\n");
	platform_device_unregister(&my_pd);
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("Naveen");
MODULE_LICENSE("GPL");
