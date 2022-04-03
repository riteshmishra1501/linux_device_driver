//we have learned device creation dynamically the last sample code
//and entry got created in /dev file but we can't open and read it as 
//file operations are not defined on it
//here we are crearting char driver and will perform basic operations like
//open , close , read , write , ioctl etc...
/*point to remember */
// struct file_operations
//cdev_init(&char_dev, &fops)  initialize cdev
//cdev_add(&char_dev, dev, 1)  add chardev 

//Author - Ritesh Mishra
//file simple_char_driver.c 

#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/major.h>
#include<linux/kdev_t.h>
#include<linux/device.h>

//creating the device entryi statically with major no 235 and minor no 0
//dev_t dev = MKDEV(235,0);

//for dynammic allocation of major and minor number
dev_t dev =0;
static struct class *dev_class;
static struct cdev char_dev;

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
	return 0 ;
}

static ssize_t my_write(struct file *fp, const char *buf, size_t len, loff_t *off){
	printk(KERN_INFO "write function is called !!!\n");
	return len;

}

static int __init hello_world_init(void) {

	if (alloc_chrdev_region(&dev, 0, 1, "RiteshMishra_dynamic") < 0){
		printk(KERN_INFO "Number could not be allocated\n");

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

	printk(KERN_INFO"MOdule successfully loaded\n");
	return 0;

remove_device :
	       class_destroy(dev_class);

remove_class : 
	       unregister_chrdev_region(dev, 1);
	       return -1;

}

static void __exit hello_world_exit(void){

   device_destroy(dev_class, dev);
   class_destroy(dev_class);   
   unregister_chrdev_region(dev, 1);
   printk(KERN_INFO "Module succesfully unloaded\n");

}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ritesh Mishra");
MODULE_DESCRIPTION("Simple char driver creating device file dynamically");

