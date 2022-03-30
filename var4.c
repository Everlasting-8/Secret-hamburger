#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Proc Dev");
MODULE_DESCRIPTION("A char driver");
MODULE_VERSION("0.1");

#define BASEMINOR 0

static struct proc_dir_entry* entry;
dev_t devno;
#define NAME      "var4"
#define  PROCFS_MAX_SIZE   256
char proc_buffer[PROCFS_MAX_SIZE];
char g_buffer[PROCFS_MAX_SIZE*10];
static int major = 207;  
static int minor = 0;
static struct class *cls;
static struct device *var4_device;
int g_count = 0;
int g_flag = 0;

static ssize_t proc_write(struct file *file, const char __user * ubuf, size_t count, loff_t* ppos) 
{
	//printk(KERN_DEBUG "Attempt to write proc file");
	g_flag = 1;
	size_t proc_buffer_size = count;
	if(proc_buffer_size > PROCFS_MAX_SIZE)
	{
		proc_buffer_size = PROCFS_MAX_SIZE;
	}
	memset(proc_buffer, 0x0, PROCFS_MAX_SIZE);
	if(copy_from_user(proc_buffer, ubuf, proc_buffer_size))
	{
		printk(KERN_INFO "write1 : proc_buffer : [%s]\n", proc_buffer);
		return -EFAULT;
	}
	printk(KERN_INFO "write2  : proc_buffer : [%s]\n", proc_buffer);
	return proc_buffer_size;
}

static ssize_t proc_read(struct file *file, char __user * ubuf, size_t count, loff_t* ppos) 
{
	int len = strlen(proc_buffer);
	int i = 0;
	g_count = 0;
	
	if (*ppos > 0 || count < len)
	{
		return 0;
	}

	if (g_flag == 1)
	{
		for (i = 0; i < len; i++)
		{
			if (proc_buffer[i] == ' ')
			{
				g_count++;
			}
		}

		sprintf(proc_buffer, "%d", g_count);
		if (strlen(g_buffer) != 0)
			strcat(g_buffer, " ");
		strcat(g_buffer, proc_buffer);

		memset(proc_buffer, 0x0, PROCFS_MAX_SIZE);
		strcpy(proc_buffer, g_buffer);
		g_flag = 0;
	}

	
	if (copy_to_user(ubuf, proc_buffer, strlen(proc_buffer)) != 0)
	{
		return -EFAULT;
	}
	*ppos = len;
	return len;
}

loff_t proc_llseek (struct file *filp,loff_t offset,int whence)
{
	loff_t newpos;
	
	switch(whence)
	{
		case 0:
			newpos = offset;
			break;
		case 1:
			newpos = filp->f_pos + offset;
			break;
		case 2:
			newpos = PROCFS_MAX_SIZE - 1 + offset;
			break;
		default:
			return -EINVAL;
	}
	
	if((newpos < 0) || (newpos > PROCFS_MAX_SIZE))
	{
		return -EINVAL;
	}
	
	filp->f_pos = newpos;
	
	return newpos;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = proc_read,
	.write = proc_write,
	.llseek = proc_llseek,
};


static struct proc_ops  proc_fops = {
	.proc_read = proc_read,
	.proc_write = proc_write,
	.proc_lseek = proc_llseek,
};
static int __init proc_var4_init(void)
{	
	int ret = 0;
	entry = proc_create("var4", 0444, NULL, &proc_fops);
	printk(KERN_INFO "%s: proc file is created\n", THIS_MODULE->name);

	devno = MKDEV(major, minor);
	ret = register_chrdev(major, "var4", &fops);

	cls = class_create(THIS_MODULE, "var4");
    if(IS_ERR(cls))
    {
        unregister_chrdev(major, "var4");
        return -EBUSY;
    }
    var4_device = device_create(cls, NULL, devno, NULL, "var4");
    if(IS_ERR(var4_device))
    {
        class_destroy(cls);
        unregister_chrdev(major,"var4");
        return -EBUSY;
    }

	
	return 0;
}

static void __exit proc_var4_exit(void)
{
	proc_remove(entry);
	device_destroy(cls, devno);
	class_destroy(cls);
	unregister_chrdev(major, "var4");
	printk(KERN_INFO "%s: proc file is deleted\n", THIS_MODULE->name);
}

module_init(proc_var4_init);
module_exit(proc_var4_exit);


