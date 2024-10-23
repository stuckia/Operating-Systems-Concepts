#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <linux/limits.h>
#include <fcntl.h>

// Name: Abby
// Date: 10/19/2024
// Description: Print a "message of the day" on terminal start
void motd() {
    // obtain current date/time
    time_t t;
    time (&t);

    // print msg and date/time
    system("clear");
    printf("\x1b[34mProgram running at %s\x1b[0m\n", ctime(&t));
    printf("\x1b[34mHope you are having a fantastic day!\x1b[0m\n");
}

// Name: Abby
// Date: 10/21/2024
// Description: Print current working directory
void print_cwd() {
    // get current working directory
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    // print current working directory
    printf("\x1b[1;34m%s>\x1b[0m", cwd);
}

// Name: Abby
// Date: 10/21/2024
// Description: Change active directory
void change_dir(char* cmd) {
    // store path of home variable
    char* home = getenv("HOME");

    // reroute to home directory if user only input "cd"
    if(strcmp(cmd, "cd") == 0) {
        chdir(home);
        return;
    }

    // pointer to start of path
    char* user = cmd + 3;

    // handle directory location issues
    if(chdir(user) != 0) {
        printf("\x1B[31mDirectory unavailable\x1B[0m\n");
    }
}

// Name: Abby
// Date: 10/22/2024
// Description: Execute user commands that are not "exit" or "cd"
void exec_user_cmd(char* cmd, int daemon) {
    int active_file;
    char* in_file = NULL;
    char* out_file = NULL;

    // check for input redirection
    if(strstr(cmd, "<") != NULL) {
        // tokenize the cmd string
        in_file = strtok(cmd, "<");
        in_file = strtok(NULL, " ");

        // retrieve file name if present
        if(in_file != NULL) {
            while(*in_file == ' ') {
                in_file++;
            }
        }
    }

    // check for output redirection
    if(strstr(cmd, ">") != NULL) {
        // tokenize the cmd string
        out_file = strtok(cmd, ">");
        out_file = strtok(NULL, " ");

        // retrieve file name if present
        if(out_file != NULL) {
            while(*out_file == ' ') {
                out_file++;
            }
        }
    }

    // fork process so child may run the user command
    int pid = fork();
    if(pid == 0) {
        if(in_file != NULL) {
            // open file with read-only permission
            active_file = open(in_file, O_RDONLY);

            // handle if file could not be opened
            if(active_file == -1) {
                printf("File unavailable");
                exit(EXIT_FAILURE);
            }

            // copy content from terminal input to file
            dup2(active_file, STDIN_FILENO);
            close(active_file);
        }

        if(out_file != NULL) {
            // open file with write-only permission or else create file
            active_file = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

            // hand if file could not be opened
            if(active_file == -1) {
                printf("File unavailable");
                exit(EXIT_FAILURE);
            }

            // copy content from terminal output to file
            dup2(active_file, STDOUT_FILENO);
            close(active_file);
        }

        // execute user command
        char* args[] = {"/bin/sh", "-c", cmd, NULL};
        if(execvp(args[0], args) == -1) {
            printf("Execution failed");
        }
        exit(EXIT_FAILURE);
    }
    // parent process waits for child process or daemon ending
    else if(pid > 0) {
        if(!daemon) {
            printf("\x1B[32m--------------------Starting program--------------------\x1B[0m\n");
            wait(NULL);
            printf("\x1B[32m--------------------Program ended--------------------\x1B[0m\n");
        }
    }
}

// Name: Abby
// Date: 10/19/2024
// Description: Main method to execute entire program
int main() {
    // execute "message of the day" on start
    motd();

    // allow buffer space
    char user_in[512];
    while(1) {
        // print current working directory
        print_cwd();

        // retrieve user input, remove newline
        fgets(user_in, 512, stdin);
        for(int c=0; c<512; c++) {
            if(user_in[c] == '\n') {
                user_in[c] = '\0';
            }
        }

        // properly execute user command
        if(strcmp(user_in, "exit") == 0) {
            printf("Program shutting down");
            break;
        }
        else if(strncmp(user_in, "cd", 2) == 0) {
            change_dir(user_in);
        }
        else {
            int daemon = (user_in[strlen(user_in)-1] == '&');
            if (daemon) {
                user_in[strlen(user_in)-1] = '\0';
            }
            exec_user_cmd(user_in, daemon);
        }
    }
}