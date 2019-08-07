#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>

static struct timer_list my_timer;

void my_timer_callback(struct timer_list* timer)
{
	printk("timer callback called\n");
}

static int __init my_init(void)
{
	int retval;
	printk("timer module loaded\n");

	//setup the timer
	timer_setup(&my_timer,my_timer_callback,0);

	//set the expiration time - 1s
	retval = mod_timer(&my_timer,jiffies+msecs_to_jiffies(300));
	if(retval)
		printk("timer expiration time failed\n");
	return 0;
}

static void __exit my_exit(void)
{
	int retval;
	printk("unloading the timer module\n");
	retval = del_timer(&my_timer);
	if(retval)
		printk("timer is still in use\n");
	printk("timer module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("Naveen Kumar Chaudhary");
MODULE_LICENSE("GPL");
