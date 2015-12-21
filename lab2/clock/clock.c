#include <linux/module.h>	/* Specifically, a module */
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/proc_fs.h>	/* Necessary because we use the proc fs */
#include <asm/uaccess.h>	/* for copy_from_user */
#include <linux/errno.h>	/* Error numbers and errno */
#include <linux/time.h>		/* for kernel time functions */
#include <linux/slab.h>		/* For kmalloc */
#include <linux/rtc.h>		/* For rtc time function */

#define CLOCK_PROC_FILE "clock"

// Time formatting related
#define YEAR_LEN 4
#define MON_DAY_LEN 2
#define SEP_LEN 1
#define HR_MIN_SEC_LEN 2 
#define TS_LEN (YEAR_LEN + 2*MON_DAY_LEN + 3*HR_MIN_SEC_LEN + 5*SEP_LEN + 4)

MODULE_AUTHOR("Praveen Kumar Pendyala");
MODULE_DESCRIPTION("Lab 2 Solution Clock driver");
MODULE_LICENSE("GPL");

// To trigger end of file while reading
int eof = 0;

// Read clock call
static ssize_t clock_read(struct file *file, char *user_buf, size_t count,
						loff_t *ppos)
{
	char *ts;
	struct timeval time;
	unsigned long local_time;
	struct rtc_time tm;
	
	// Signal eof if previous read was done.
	if(eof == 1)
		return 0;
		
	ts = kmalloc(sizeof(*ts) * TS_LEN, GFP_KERNEL);
	
	// Get current time
	do_gettimeofday(&time);
	
	// Covert to local time
	local_time = (u32)(time.tv_sec - (sys_tz.tz_minuteswest * 60));
	rtc_time_to_tm(local_time, &tm);
	
	// Format the time. +1 for hours is to get to EST time zone
	sprintf(ts, "%04d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, 
			tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, 
			tm.tm_sec);
	
	// Copy to userspace
	copy_to_user(user_buf, ts, TS_LEN);
	
	// Set eof to 1 for next reads and free memory
	eof = 1;
	kfree(ts);
	
	return TS_LEN;
}

// Write clock call - unsupported
static ssize_t clock_write( struct file *file, const char *buf, size_t count,
						 loff_t *ppos )			
{
	printk("Clock write operation not supported\n");
	return -EPERM;
}

// Open device call
static int clock_open(struct inode * inode, struct file * file)
{
	printk("Clock file opened!\n");
	eof = 0;
	return 0;	
}


// Release clock for future operations
static int clock_release(struct inode * inode, struct file * file)
{
	return 0;
}

// clocks' file operations structure
static struct file_operations clock_fops = {
	.owner =	THIS_MODULE,
	.read =		clock_read,
	.write =	clock_write,
	.open =		clock_open,
	.release =	clock_release,
};

// Initialize clock device on insmod
static int __init clock_device_init(void)
{	
	proc_create(CLOCK_PROC_FILE, 0, NULL, &clock_fops);
	return 0;
}

// Remove clock device on rmmod
static void __exit clock_device_cleanup(void)
{
	remove_proc_entry(CLOCK_PROC_FILE, NULL);
}

module_init(clock_device_init);
module_exit(clock_device_cleanup);
