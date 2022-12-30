#include <linux/kernel.h>
#include <linux/module.h>

static int hello_init(void)
{
    printk("module init");

    return 0;
}

static void hello_exit(void)
{
    printk("module exit");
}

MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
