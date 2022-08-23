/* 
 * procfs3.c 
 */ 
 
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/proc_fs.h> 
#include <linux/sched.h> 
#include <asm/uaccess.h>
#include <linux/version.h> 
 
 
#define PROCFS_MAX_SIZE 2048 
#define PROCFS_ENTRY_FILENAME "buffer2k" 
 
static struct proc_dir_entry *our_proc_file; 
static char procfs_buffer[PROCFS_MAX_SIZE]; 
static unsigned long procfs_buffer_size = 0; 
 
static ssize_t procfs_read(struct file *filp, char __user *buffer, 
                           size_t length, loff_t *offset) 
{ 
    static int finished = 0; 
 
    if (finished) { 
        pr_info("procfs_read: END\n"); 
        finished = 0; 
        return 0; 
    } 
    finished = 1; 
 
    if (copy_to_user(buffer, procfs_buffer, procfs_buffer_size)) 
        return -EFAULT; 
 
    pr_info("procfs_read: read %lu bytes\n", procfs_buffer_size); 
    return procfs_buffer_size; 
} 
static ssize_t procfs_write(struct file *file, const char __user *buffer, 
                            size_t len, loff_t *off) 
{ 
    if (len > PROCFS_MAX_SIZE) 
        procfs_buffer_size = PROCFS_MAX_SIZE; 
    else 
        procfs_buffer_size = len; 
    if (copy_from_user(procfs_buffer, buffer, procfs_buffer_size)) 
        return -EFAULT; 
 
    pr_info("procfs_write: write %lu bytes\n", procfs_buffer_size); 
    return procfs_buffer_size; 
} 

static int module_permission(struct inode *inode, int op, struct nameidata *nd)
{
	/*
	 * We allow everybody to read from our module, but
	 * only root (uid 0) may write to it
	 */
	if (op == 4 || (op == 2 && current->cred->uid == 0))
		return 0;

	/*
	 * If it's anything else, access is denied
	 */
	return -EACCES;
}
static int procfs_open(struct inode *inode, struct file *file) 
{ 
    try_module_get(THIS_MODULE); 
    return 0; 
} 
static int procfs_close(struct inode *inode, struct file *file) 
{ 
    module_put(THIS_MODULE); 
    return 0; 
} 

static const struct file_operations file_ops_4_our_proc_file = { 
    .read = procfs_read, 
    .write = procfs_write, 
    .open = procfs_open, 
    .release = procfs_close, 
};  
 
static struct inode_operations Inode_Ops_4_Our_Proc_File = {
	.permission = module_permission,	/* check for permissions */
};
static int __init procfs3_init(void) 
{ 
    our_proc_file = proc_create(PROCFS_ENTRY_FILENAME, 0644, NULL, 
                                &file_ops_4_our_proc_file); 
    if (our_proc_file == NULL) { 
        remove_proc_entry(PROCFS_ENTRY_FILENAME, NULL); 
        pr_info("Error: Could not initialize /proc/%s\n", 
                 PROCFS_ENTRY_FILENAME); 
        return -ENOMEM; 
    } 
    //proc_set_size(our_proc_file, 80); 
    //proc_set_user(our_proc_file, GLOBAL_ROOT_UID, GLOBAL_ROOT_GID);
    our_proc_file->owner = THIS_MODULE;
	our_proc_file->proc_iops = &Inode_Ops_4_Our_Proc_File;
	our_proc_file->proc_fops = &file_ops_4_our_proc_file;
	our_proc_file->mode = S_IFREG | S_IRUGO | S_IWUSR;
	our_proc_file->uid = 0;
	our_proc_file->gid = 0;
	our_proc_file->size = 80;

//	printk(KERN_INFO "/proc/%s created\n", PROC_ENTRY_FILENAME);

//	return 0;	
 
    pr_info("/proc/%s created\n", PROCFS_ENTRY_FILENAME); 
    return 0; 
} 
 
static void __exit procfs3_exit(void) 
{ 
    remove_proc_entry(PROCFS_ENTRY_FILENAME, NULL); 
    pr_info("/proc/%s removed\n", PROCFS_ENTRY_FILENAME); 
} 
 
module_init(procfs3_init); 
module_exit(procfs3_exit); 
 
MODULE_LICENSE("GPL");
