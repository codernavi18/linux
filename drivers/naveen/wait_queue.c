#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/workqueue.h>

/* declare wait queue head */
static DECLARE_WAIT_QUEUE_HEAD(my_wq);
/* declare condition */
static int condition = 0;

/* declare work - this defines the work to be done */
static struct work_struct work;

/* actual work to be done - handler for the above struct */
static void work_handler(struct work_struct *work)
{
	printk("work handler executing\n");
	msleep(5000);
	printk("wake up the sleeping module\n");
	//set the condition to be true
	condition = 1;
	//wake the first process in the wait queue
	wake_up_interruptible(&my_wq);
}

static int __init my_init(void)
{
	printk("Wait Queue module loaded\n");

	INIT_WORK(&work, work_handler);
	/* this tells the kernel to start doing the defined work
	 * Note that we have not defined our work queue.
	 * By default, kernel event threads perform this work
	 */
	schedule_work(&work);

	//Now since we have scheduled the work, we will go to indefinite sleep mode.
	printk("Going to wait indefinitely, unless interrupted\n");
	wait_event_interruptible(my_wq, condition != 0);

	pr_info("Woken up by the work job\n");
	return 0;
}

static void __exit my_exit(void)
{
	printk("wait queue module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("Naveen Kumar Chaudhary <naveenchaudhary2010@hotmail.com>");
MODULE_LICENSE("GPL");
