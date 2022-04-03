//we have learned what major and minor numbers are in the last sample code
//now we will create a entry of the same driver in /dev directory
//here we are crearting the device file dynamically using
//step 1-> call create_class() --> it will return the struct class in /sys directory
//step 2 -> call device_create() --> it will create a device using the above class 
//NOTE :: add linux/kdev.h & linux/device.h


//Author - Ritesh Mishra
//file device_file_dynamic.c
//major no --> specific to a driver being used
//minor no - number of devices usingthe same driver

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

static int __init hello_world_init(void) {

	//register_chrdev_region(dev, 1, "RiteshMishra");
	if (alloc_chrdev_region(&dev, 0, 1, "RiteshMishra_dynamic") < 0){
		printk(KERN_INFO "Number could not be allocated\n");

	}
	printk(KERN_INFO "major number is %d minor number is %d\n", MAJOR(dev), MINOR(dev));

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

