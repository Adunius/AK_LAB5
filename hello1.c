#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include "hello1.h"

MODULE_AUTHOR("Andrii Pozdniakov");
MODULE_DESCRIPTION("Hello1 Module");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_event {
    struct list_head list;
    ktime_t start_time;
    ktime_t end_time;
};

static LIST_HEAD(hello_list);

void print_hello(void)
{
    struct hello_event *event;

    event = kmalloc(sizeof(*event), GFP_KERNEL);
    if (!event) {
        pr_err("Failed to allocate memory for hello_event\n");
        return;
    }

    event->start_time = ktime_get();
    pr_info("Hello, world!\n");
    event->end_time = ktime_get();

    list_add_tail(&event->list, &hello_list);
}
EXPORT_SYMBOL(print_hello);

static void __exit hello1_exit(void)
{
    struct hello_event *event, *tmp;

    list_for_each_entry_safe(event, tmp, &hello_list, list) {
        pr_info("Print duration: %llu ns\n", 
                ktime_to_ns(event->end_time) - ktime_to_ns(event->start_time));
        list_del(&event->list);
        kfree(event);
    }
}

static int __init hello1_init(void)
{
    pr_info("Hello1 module loaded\n");
    return 0;
}

module_init(hello1_init);
module_exit(hello1_exit);

