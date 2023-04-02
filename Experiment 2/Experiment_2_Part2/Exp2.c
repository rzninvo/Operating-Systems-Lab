#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>

struct birthday{
int day;
int month;
int year;
struct list_head list;
};

static LIST_HEAD(birthday_list);

int simple_init(void)
{
	
	int i;
	for (i = 0 ; i < 5; i++)
	{
		struct birthday *person;
		person = kmalloc(sizeof(person), GFP_KERNEL);
		person->day = i;
		person->month = 8;
		person->year = 1995;
		INIT_LIST_HEAD(&person->list);
		list_add_tail(&person->list, &birthday_list);
	}
	printk(KERN_INFO "Hello Boiii\n");
	return 0;
}

void simple_exit(void)
{
	struct birthday *ptr;
	list_for_each_entry(ptr, &birthday_list, list) {
		printk(KERN_INFO "Person%d\n", ptr->day);
	}
}


module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("IT'S YA BOY");
MODULE_AUTHOR("SGG");
