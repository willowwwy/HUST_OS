#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/poll.h>

#define DEVICE_NAME "my_dv"
static int rwdMajor = 0;
static int MAX_BUF_LEN = 1024;
static char drv_buf[1024];
static int WRI_LENGTH = 0;

int myatoi(char *s, int *len)
{
	int ret = 0;
	int i = 0;
	while (s[i] != ' ' && s[i] != '\0'){
		ret = ret * 10 + s[i] - '0';
		i++;
	}
	*len = i + 1;
	return ret;
}
static ssize_t dv_write(struct file *filp, const char __user *buffer, size_t count, loff_t *ppos)
{
	if (count > MAX_BUF_LEN)
		count = MAX_BUF_LEN;
	copy_from_user(drv_buf, buffer, count);
	WRI_LENGTH = count;
	printk("user write data to driver\n");

	int len = 0;
	int temp = myatoi(drv_buf, &len) + myatoi(drv_buf + len, &len);
	sprintf(drv_buf, "%d", temp);
	return count;
}
static ssize_t dv_read(struct file *filp, char __user *buffer, size_t count, loff_t *ppos)
{
	if (count > MAX_BUF_LEN)
		count = MAX_BUF_LEN;
	copy_to_user(buffer, drv_buf, count);
	printk("user read data from driver\n");
	return count;
}

static int dv_open(struct inode *inode, struct file *filp)
{
	printk("Device open success£¡\n");
	return 0;
}
static int dv_release(struct inode *inode, struct file *filp)
{
	printk("device release£¡\n");
	return 0;
}

static struct file_operations pxa270_fops = {
	.owner = THIS_MODULE,
	.write = dv_write,
	.read = dv_read,
	.open = dv_open,
	.release = dv_release,
};

static int __init dv_init(void)
{
	int ret;
	ret = register_chrdev(0, DEVICE_NAME, &pxa270_fops);
	if (ret < 0)
	{
		printk(DEVICE_NAME "can't get major number\n");
		return ret;
	}
	rwdMajor = ret;

	printk("dv module major number is %d\n", ret);
	return 0;
}

void __exit dv_exit(void)
{
	unregister_chrdev(rwdMajor, DEVICE_NAME);
}

module_exit(dv_exit);
module_init(dv_init);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("WWY");

