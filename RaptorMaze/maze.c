#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

//module metadata
MODULE_AUTHOR("Abby Stucki");
MODULE_DESCRIPTION("Raptor Maze generator");
MODULE_LICENSE("GPL");

//init method
static int __init maze_init(void) {
	printk(KERN_INFO "Raptor Maze generating...");
	return 0;
}

//exit method
static void __exit maze_exit(void) {
	printk(KERN_INFO "Raptor Maze closing...");
}

module_init(maze_init);
module_exit(maze_exit);
