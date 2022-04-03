//simple driver which can read the data from kernal and write the data to kernel using
//copy_to_user and copy_from_user
//we are going to see simple use of kmalloc and kfree functions
//Also going to add user spcae application which will  read & write the data tfrom and to the kernal 
//using open read and write system calls
//

/*point to remember */

//Author - Ritesh Mishra
//file simple_char_driver.c 

#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/kdev_t.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>

#define mem_size     1024


//for dynammic allocation of major and minor number
dev_t dev =0;
static struct class *dev_class;
static struct cdev char_dev;
char *kernel_buffer;

//function prototype
static int  __init hello_world_init(void);
static void  __exit hello_world_exit(void);
static int my_open(struct inode *inode, struct file *file);
static int my_close(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *fp, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write(struct file *fp, const char *buf, size_t len, loff_t *off);

//file operaion structure
static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.read  = my_read,
	.write = my_write,
	.open = my_open,
	.release = my_close,
};

static int my_open(struct inode *inode, struct file *file) {
	printk(KERN_INFO "Open function is called\n");
	return 0;

}

static int my_close(struct inode *inode, struct file *file){
	printk(KERN_INFO "close function is called\n");
	return 0;

}

static ssize_t my_read(struct file *fp, char __user *buf, size_t len, loff_t *off){

	printk(KERN_INFO "My read function is called !!!\n");
	if (copy_to_user(buf, kernel_buffer, mem_size)){
		printk(KERN_INFO "data read error !!");
	}
	printk(KERN_INFO "Data read DONE !!");
	return mem_size ;
}

static ssize_t my_write(struct file *fp, const char *buf, size_t len, loff_t *off){
	printk(KERN_INFO "write function is called !!!\n");

	if (copy_from_user(kernel_buffer, buf,  len)){
                printk(KERN_INFO "Data write error !!");
        }
        printk(KERN_INFO "Data write DONE !!");

	return len;

}

static int __init hello_world_init(void) {

	if (alloc_chrdev_region(&dev, 0, 1, "RiteshMishra_dynamic") < 0){
		printk(KERN_INFO "Number culd not be allocated\n");

	}
	printk(KERN_INFO "major number is %d minor number is %d\n", MAJOR(dev), MINOR(dev));
	//create cdev structure
	cdev_init(&char_dev, &fops);

	//adding character device to the system 
	if (cdev_add(&char_dev, dev, 1) < 0){
		printk(KERN_INFO "cant add char device in system\n");
		goto remove_class;

	}

	//creating struct class
	dev_class = class_create(THIS_MODULE, "my_class");
	if(dev_class == NULL){
		printk(KERN_INFO "class could not be created\n");
		goto remove_class;
	}

	//create device of same class
	if(device_create(dev_class, NULL, dev, NULL, "my_device") == NULL ){
		printk(KERN_INFO "Could not create device\n");
		goto remove_device;
	}

	//allocate kernel memory
	kernel_buffer = kmalloc(mem_size, GFP_KERNEL);
	if(kernel_buffer == NULL){
		printk(KERN_INFO "memory cant allocated\n");
		goto remove_device;

	}

	strcpy(kernel_buffer, "Ritesh is writing char driver");

	printk(KERN_INFO"MOdule successfully loaded\n");
	return 0;

remove_device :
	class_destroy(dev_class);

remove_class : 
	unregister_chrdev_region(dev, 1);
	return -1;

}

static void __exit hello_world_exit(void){

	kfree(kernel_buffer);
	device_destroy(dev_class, dev);
	class_destroy(dev_class);   
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "Module succesfully unloaded\n");

}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ritesh Mishra");
MODULE_DESCRIPTION("full fledged char driver");

