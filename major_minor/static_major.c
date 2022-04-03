//major and minir number tutorials
//writer - Ritesh Mishra
//file static_major.c
//major no --> specific to a driver being used
//minor no - number of devices usingthe same driver

#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/major.h>

//creating the device entry with major no 235 and minor no 0
dev_t dev = MKDEV(235,0);

//for dynammic allocation of major and minor number
//dev_t dev =0;


static int __init hello_world_init(void){

	register_chrdev_region(dev, 1, "RiteshMishra");
	//alloc_chrdev_region(&dev, 0, 1, "RiteshMishra_dynamic");
	printk(KERN_INFO "major number is %d minor number is %d\n", MAJOR(dev), MINOR(dev));
	printk(KERN_INFO"MOdule successfully loaded\n");
	return 0;


}

static void __exit hello_world_exit(void){
   unregister_chrdev_region(dev, 1);
   printk(KERN_INFO "Module succesfully unloaded\n");

}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ritesh Mishra");

