// maze generated via prim algorithm
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/random.h>

// module metadata
MODULE_AUTHOR("Abby Stucki");
MODULE_DESCRIPTION("Raptor Maze generator");
MODULE_LICENSE("GPL");

#define rows 15
#define cols 15
#define dim 225

typedef struct Frontier {
    // 0=north, 1=south, 2=east, 3=west
    int direction;
} Frontier;

static int get_rand(int max);
static struct proc_dir_entry* proc_entry;

// Name: Abby
// Date: 9/18/2024
// Description: Generate random number within range of 0 to max
static int get_rand(int max) {
    int i, rand_num;
    get_random_bytes(&i, sizeof(i));
    rand_num = i % max;
    return ((rand_num >= 0) ? rand_num : -rand_num);
}

// Name: Abby
// Date: 9/23/2024
// Description: Main function of process, creates maze
static ssize_t create_maze(struct file* file, char __user* user_buffer, size_t count, loff_t* offset) {
    struct Frontier maze[dim];
    int visited[dim];
    int frontier[dim];
    int added_to_frontier[dim];
    int frontier_size = 0;
    int curr_index = 0;
    int random_num = 0;
    int frontier_index = 0;
    int temp_rand = 0;
    char finalarr[(rows * 2) + 1][(cols * 2) + 1];
    char arr_flat[((rows * 2) + 1) * ((cols * 2) + 1) + ((rows * 2) + 1)];
    int arr_length = ((rows * 2) + 1) * ((cols * 2) + 1) + ((rows * 2) + 1);
    int index = 0;
    int possible_frontier[4] = { 0,0,0,0 };
    int possible_size = 0;

    printk(KERN_INFO "Beginning maze generation.");
    
    for (int x = 0; x < dim; x++) {
        visited[x] = 0;
        frontier[x] = 0;
        added_to_frontier[x] = 0;
        maze[x].direction = -1;
    }
    
    // add valid neighbors to current frontier options
    do {
        visited[curr_index] = 1;

        // add north neighbor to frontier if unvisited and valid
        if (curr_index - cols >= 0 && curr_index >= cols) {
            if (!visited[curr_index - cols] && !added_to_frontier[curr_index - cols]) {
                frontier[frontier_size] = curr_index - cols;
                added_to_frontier[curr_index - cols] = 1;
                frontier_size++;
            }
        }
        // add south neighbor to frontier if unvisited and valid
        if (curr_index + cols < dim && curr_index < dim + cols) {
            if (!visited[curr_index + cols] && !added_to_frontier[curr_index + cols]) {
                frontier[frontier_size] = curr_index + cols;
                added_to_frontier[curr_index + cols] = 1;
                frontier_size++;
            }
        }
        // add east neighbor to frontier if unvisited and valid
        if (curr_index + 1 < dim && curr_index + 1 % cols > 0) {
            if (!visited[curr_index + 1] && !added_to_frontier[curr_index + 1]) {
                frontier[frontier_size] = curr_index + 1;
                added_to_frontier[curr_index + 1] = 1;
                frontier_size++;
            }
        }
        // add west neighbor to frontier if unvisited and valid
        if (curr_index - 1 >= 0 && curr_index % cols > 0) {
            if (!visited[curr_index - 1] && !added_to_frontier[curr_index - 1]) {
                frontier[frontier_size] = curr_index - 1;
                added_to_frontier[curr_index - 1] = 1;
                frontier_size++;
            }
        }

        // choose frontier cell (unvisited)
        random_num = get_rand(frontier_size);
        frontier_index = frontier[random_num];

        // find visited neighbors
        //possible_frontier = { 0,0,0,0 };
        possible_size = 0;

        // add north neighbor to frontier if visited and valid
        if (frontier_index - cols >= 0 && frontier_index >= cols) {
            if (visited[frontier_index - cols]) {
                possible_frontier[possible_size] = 0;
                possible_size++;
            }
        }
        // add south neighbor to frontier if visited and valid
        if (frontier_index + cols < dim && frontier_index < dim + cols) {
            if (visited[frontier_index + cols]) {
                possible_frontier[possible_size] = 1;
                possible_size++;
            }
        }
        // add east neighbor to frontier if visited and valid
        if (frontier_index + 1 < dim && frontier_index + 1 % cols > 0) {
            if (visited[frontier_index + 1]) {
                possible_frontier[possible_size] = 2;
                possible_size++;
            }
        }
        // add west neighbor to frontier if visited and valid
        if (frontier_index - 1 >= 0 && frontier_index % cols > 0) {
            if (visited[frontier_index - 1]) {
                possible_frontier[possible_size] = 3;
                possible_size++;
            }
        }

        // grab index of parent, assign direction
        temp_rand = get_rand(possible_size);
        maze[frontier_index].direction = possible_frontier[temp_rand];

        // clean up and update arrays
        visited[frontier_index] = 1;
        if (frontier_size - 1 >= 0) {
            frontier[random_num] = frontier[frontier_size - 1];
            frontier_size--;
        }

        curr_index = frontier_index;
    } while (frontier_size >= 1);

    // print maze
    for (int r = 0; r < (rows * 2) + 1; r++) {
        for (int c = 0; c < (cols * 2) + 1; c++) {
            finalarr[r][c] = '#';
        }
    }

    for (int i = 0; i < dim; i++) {
        int col = i % cols;
        int row = i / rows;
        col = (col * 2) + 1;
        row = (row * 2) + 1;
        finalarr[row][col] = ' ';
        switch (maze[i].direction) {
        case 0:
            if (row - 1 >= 0) {
                // parent is north
                finalarr[row - 1][col] = ' ';
            }
            break;
        case 1:
            if (row + 1 < (rows * 2) + 1) {
                // parent is south
                finalarr[row + 1][col] = ' ';
            }
            break;
        case 2:
            if (col + 1 < (cols * 2)) {
                // parent is east
                finalarr[row][col + 1] = ' ';
            }
            break;
        case 3:
            if (col - 1 >= 0) {
                // parent is west
                finalarr[row][col - 1] = ' ';
            }
            break;
        default:
            break;
        }
    }

    // final arr for printing needs to be full length plus space for \n
    for (int r2 = 0; r2 < (rows * 2) + 1; r2++) {
        for (int c2 = 0; c2 < (cols * 2) + 1; c2++) {
            arr_flat[index] = finalarr[r2][c2];
            index++;

            if (c2 == (cols * 2)) {
                arr_flat[index] = '\n';
                index++;
            }
        }
    }

    // load array into user buffer
    if (*offset > 0) {
        return 0;
    }

    copy_to_user(user_buffer, arr_flat, arr_length);
    *offset = arr_length;
    return arr_length;
}

static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .read = create_maze
};

// Name: Abby
// Date: 9/18/2024
// Description: initialize maze proc
static int __init maze_init(void) {
    proc_entry = proc_create("raptormaze", 0666, NULL, &fops);
    printk(KERN_INFO "Raptor Maze generating...");
    return 0;
}

// Name: Abby
// Date: 9/18/2024
// Description: exit maze proc
static void __exit maze_exit(void) {
    proc_remove(proc_entry);
    printk(KERN_INFO "Raptor Maze closing...");
}

module_init(maze_init);
module_exit(maze_exit);
