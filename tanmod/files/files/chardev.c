/* 
 * chardev.c: Creates a read-write char device
 */ 
 
#include <linux/cdev.h> 
#include <linux/delay.h> 
#include <linux/device.h> 
#include <linux/fs.h> 
#include <linux/ioctl.h>
#include <linux/uaccess.h> //copy to/from user()
#include <linux/init.h> //needed for macros
#include <linux/irq.h> 
#include <linux/kernel.h>  //needed for pr_info()
#include <linux/module.h>  //needed by all modules()
#include <linux/poll.h> 
#include <linux/slab.h> //kmalloc()



/*  Prototypes - this would normally go in a .h file */ 
static int device_open(struct inode *, struct file *); //static because these funs are not supposed to be used directly outside of module
static int device_release(struct inode *, struct file *); 
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *); 
static ssize_t device_write(struct file *, const char __user *, size_t, 
                            loff_t *); 
 
#define DEVICE_NAME "chardev1" /* Dev name as it appears in /proc/devices   */ 
#define BUF_LEN 180 /* Max length of the message from the device */ 
static struct cdev my_cdev;
static struct class *dev_class;
 
/* Global variables are declared as static, so are global within the file. */ 
 
static int major=0; /* major number assigned to our device driver */ 
dev_t dev=0;
 
uint8_t *kernel_buffer; /* The msg the device will give when asked */ //uint8_t -unsinged char
  
static struct file_operations fops = { 
    .read = device_read, 
    .write = device_write, 
    .open = device_open, 
    .release = device_release, 
}; 

/*
function to toggle the buffer
*/

void togglecode(uint8_t *buff,int count)

{
int i;
	for( i=0;i<count;i++)
	{
		if ((buff[i] >= 'A') && (buff[i] <= 'Z'))
		buff[i] += 32;
		else if ((buff[i] >= 'a') && (buff[i] <= 'z'))
		buff[i] -= 32;
	}
}

 
static int __init chardev_init(void) 
{ 
    

	
   //alloating major num
	if((alloc_chrdev_region(&dev,0,1,"chardev1"))  < 0){
	pr_info("cannot allocate the major num \n");
	return -1;
	}

	pr_info("major = %d minor = %d..\n",MAJOR(dev),MINOR(dev));

	//creating cdev structure
	cdev_init(&my_cdev,&fops);

	//adding char device to system
	if((cdev_add(&my_cdev,dev,1)) <0) {
		pr_info("cannot add device to system \n");
		goto r_class;

	
	}
	//creating struct class
	if((dev_class = class_create(THIS_MODULE,"my_class")) == NULL){
            pr_info("Cannot create the struct class\n");
            goto r_class;
        }
        /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"my_device")) == NULL){  // in /dev a special file will be created with name my_device
            pr_info("Cannot create the Device 1\n");
            goto r_device; 
        }

	pr_info("device inserted properly \n");
	return 0;

r_device:
        class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,1);
	return -1;

}
 
    

static void __exit chardev_exit(void) 
{ 
   
 
    /* Unregister the device */ 
   unregister_chrdev(major, DEVICE_NAME);
   pr_info("device driver removed successfully \n"); 
} 
 
/* Methods */ 
 
/* Called when a process tries to open the device file, like 
 * "sudo cat /dev/chardev" 
 */ 
static int device_open(struct inode *inode, struct file *file) 
{ 
    //creating physical memory
    if((kernel_buffer=kmalloc(BUF_LEN,GFP_KERNEL)) == 0) //kmalloc used to allocate memory in kernel , here memory allocated from kernel_ram as given flag
    {
	  pr_info("cannot allocate the memory to kernel \n ");
		return -1;
   } 
    pr_info("file opened \n");

    return 0;    

    try_module_get(THIS_MODULE); 
 
} 
 
/* Called when a process closes the device file. */ 
static int device_release(struct inode *inode, struct file *file) 
{ 
    kfree(kernel_buffer); //free previously allocated memory
    pr_info("device file closed");

    module_put(THIS_MODULE); 
 
    return 0; 
} 
 
/* Called when a process, which already opened the dev file, attempts to 
 * read from it. 
 */ 
static ssize_t device_read(struct file *filp, /* see include/linux/fs.h   */ 
                           char __user *buffer, /* buffer to fill with data */ 
                           size_t length, /* length of the buffer     */ 
                           loff_t *offset) 
	//ssize_t is signed size_t data type used for read n write syscall which can return -1 returned by read n write calls several times
{ 
   togglecode(kernel_buffer,strlen(kernel_buffer));
   copy_to_user(buffer,kernel_buffer,BUF_LEN); //copy buf_len no of bytes from kernel to usrr space

   pr_info("data read \n");
	return BUF_LEN; 
} 
 
/* Called when a process writes to dev file: echo "hi" > /dev/hello */ 
static ssize_t device_write(struct file *filp, const char __user *buff, 
                            size_t len, loff_t *off) 
{ 
    

copy_from_user(kernel_buffer,buff,len); //copy len bytes from user to kernel

pr_info("data is wriiten \n");

return len;

} 
 
module_init(chardev_init); 
module_exit(chardev_exit); 
 
MODULE_LICENSE("GPL");
