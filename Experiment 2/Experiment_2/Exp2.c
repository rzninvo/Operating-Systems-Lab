#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

int simple_init(void)
{
	printk(KERN_INFO "Hello Negro\n");
	return 0;
}

void simple_exit(void)
{
	printk(KERN_INFO "Removing Module Experiment2\n");
}


module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("IT'S YA BOY");
MODULE_AUTHOR("SGG");
