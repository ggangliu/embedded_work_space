#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>

static struct cdev chr_dev; //define a char device
static dev_t ndev; //char device node's device number

static int chr_open(struct inode *nd, struct file *file){
	int major = MAJOR(nd->i_rdev);
	int minor = MINOR(nd->i_rdev);
	printk("chr_open, major=%d, minor=%d\n", major, minor);
	return 0;
}

static ssize_t chr_read(struct file *fl, char __user *u, size_t sz, loff_t *off){
	printk("In the chr_read() function!\n");
	if (copy_to_user(u, "I am from kernel chr_read().\n", sz)){
		printk("copy_to_user maybe happened fault!\n");
		return -EINVAL;
	}
	
	return 0;
}

static ssize_t chr_write(struct file *fl, const char __user *u, size_t sz, loff_t *off){
	char buf[33] = {0};
	
	printk("In the chr_write() function!\n");
	if (copy_from_user(buf, u, sz)){
		printk("copy_from_user maybe happened fault!\n");
		return -EINVAL;
	}
	printk("I am from user space \"%s\"", buf);
	return 0;
}

struct file_operations chr_ops = {
	.owner = THIS_MODULE,
	.open  = chr_open,
	.read  = chr_read,
	.write = chr_write,
};

static int demo_init(void){
	int ret = 0;
	cdev_init(&chr_dev, &chr_ops); //initial char device object
	if ((ret = alloc_chrdev_region(&ndev, 0, 1, "chr_dev")) < 0){ //alloc device number
		return ret;
	}
	
	printk("demo_init() : major=%d, minor=%d\n", MAJOR(ndev), MINOR(ndev));
	if ((ret = cdev_add(&chr_dev, ndev, 1)) <0){ //register char device into system
		return ret;
	}
	
	return 0;
}

static void demo_exit(void){
	printk("Removing chr_dev module...\n");
	cdev_del(&chr_dev); //del char device from system
	unregister_chrdev_region(ndev, 1); //release device number
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GGang.liu@LinuxFGL");
MODULE_DESCRIPTION("A char device driver as an example");
