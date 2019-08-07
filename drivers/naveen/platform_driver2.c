#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>

struct my_gpios
{
	int reset_gpio;
	int led_gpio;
};

static int my_pdrv_probe(struct platform_device *pdev)
{
	pr_info("platform device probe called\n");

	//fetch the platform data
	struct my_gpios *my_gpio_data = (struct my_gpios*)dev_get_platdata(&pdev->dev);
	pr_info("reset gpio : %d led gpio : %d",my_gpio_data->reset_gpio, my_gpio_data->led_gpio);

	//fetch the resource data
	struct resource *res1, *res2;
	int irqnum;
	res1 = platform_get_resource(pdev, IORESOURCE_MEM,0);
	if(!res1)
	{
		pr_err("error getting resource 1 info\n");
		return -1;
	}
	pr_info("start address : %lld end address : %lld name : %s",res1->start,res1->end,res1->name);

	res2 = platform_get_resource(pdev,IORESOURCE_MEM,1);
	if(!res2)
	{
		pr_err("error getting resource 2 info\n");
		return -1;
	}
	pr_info("start address : %lld end address : %lld name : %s",res2->start,res2->end,res2->name);

	irqnum = platform_get_irq(pdev,0);
	pr_info("IRQ number : %d\n",irqnum);
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
