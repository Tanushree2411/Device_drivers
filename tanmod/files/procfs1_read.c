/* 
 * procfs1.c 
 */ 
 
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/proc_fs.h> 
#include <linux/uaccess.h> 
#include <linux/version.h> 
 
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0) 
#define HAVE_PROC_OPS 
#endif 
 
#define procfs_name "helloworld" 
 
static struct proc_dir_entry *our_proc_file; 
static int count = 0; 
static ssize_t procfile_read(struct file *filePointer, char __user *buffer, 
                             size_t buffer_length, loff_t *offset) 
{ 
    char s[] = "HelloWorld!\n"; 
    int len = sizeof(s); 
    int ret;
    count++; 
  //  pr_info("count: %d len:%d offset:%d\n", count, len, *offset); 
 
    if (*offset > 0 ) {
	  // pr_info("going to if loop\n"); 
        pr_info("copy_to_user failed\n"); 
        ret = 0;
       pr_info("valoe of ret is %d\n",ret);	
    }
    else
    { 
        //pr_info("procfile read %s\n", filePointer->f_path.dentry->d_name.name); 
    	copy_to_user(buffer, s, len);
	 pr_info("procfile read %s\n", filePointer->f_path.dentry->d_name.name);

        //*offset += len; 
	ret = len;
	pr_info("value of ret is %d\n",ret);
    } 
 
 //   pr_info("len:%d offset:%d\n", len, *offset); 
 return ret; 
} 
 
#ifdef HAVE_PROC_OPS 
static const struct proc_ops proc_file_fops = { 
    .proc_read = procfile_read, 
}; 
#else 
static const struct file_operations proc_file_fops = { 
    .read = procfile_read, 
}; 
#endif 
 
static int __init procfs1_init(void) 
{ 
    our_proc_file = proc_create(procfs_name, 0644, NULL, &proc_file_fops); 
    if (NULL == our_proc_file) { 
        proc_remove(our_proc_file); 
        pr_alert("Error:Could not initialize /proc/%s\n", procfs_name); 
        return -ENOMEM; 
    } 
 
    pr_info("/proc/%s created\n", procfs_name); 
    return 0; 
} 
 
static void __exit procfs1_exit(void) 
{ 
    proc_remove(our_proc_file); 
    pr_info("/proc/%s removed\n", procfs_name); 
} 
 
module_init(procfs1_init); 
module_exit(procfs1_exit); 
 
MODULE_LICENSE("GPL");
