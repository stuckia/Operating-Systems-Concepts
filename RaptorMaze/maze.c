#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/random.h>

//module metadata
MODULE_AUTHOR("Abby Stucki");
MODULE_DESCRIPTION("Raptor Maze generator");
MODULE_LICENSE("GPL");

//Name: Abby
//Date: 9/18/2024
//Description: Generate random number within range of 0 to max
static int get_random(int max) {
	int i, rand_num;
	get_random_bytes(&i, sizeof(i));
	rand_num = i % max;
	return ((rand_num >= 0) ? rand_num : -rand_num);
}

//init method
static int __init maze_init(void) {
	printk(KERN_INFO "Raptor Maze generating...");
	printk(KERN_INFO "Random number generated: %d", get_random(100));
	return 0;
}

//exit method
static void __exit maze_exit(void) {
	printk(KERN_INFO "Raptor Maze closing...");
}

module_init(maze_init);
module_exit(maze_exit);
