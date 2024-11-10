#include <stdio.h>
#include <stdbool.h>

void fifo_scheduler(int n, char processes[][10], int burst[], int arrival[]) {
    // order by arrival time, bubble sort
    bool swap;
    for(int i=0; i<n-1; i++) {
        swap = false;
        for(int j=0; j<n-1; j++) {
            if(arrival[j] > arrival[j+1]) {
                int temp_i = arrival[j];
                arrival[j] = arrival[j+1];
                arrival[j+1] = temp_i;

                temp_i = burst[j];
                burst[j] = burst[j+1];
                burst[j+1] = temp_i;

                for(int k=0; k<10; ++k) {
                    char temp = processes[j][k];
                    processes[j][k] = processes[j+1][k];
                    processes[j+1][k] = temp;
                }
                
                swap = true;
            }
        }

        if(swap == false) {
            break;
        }
    }
    
    // calculate completion[], turnaround[], wait[], response[]
    int completion[n];
    int turnaround[n];
    int wait[n];
    int response[n];
    for(int i=0; i<n; i++) {
        if(i==0) {
            completion[0] = burst[0] + arrival[0];
        }
        else {
            int max = (((completion[i-1]) > (arrival[i])) ? (completion[i-1]) : (arrival[i]));
            completion[i] = max + burst[i];
        }
        turnaround[i] = completion[i] - arrival[i];
        wait[i] = turnaround[i] - burst[i];
        response[i] = wait[i];
        printf("\nProcess   : %s\n", processes[i]);
        printf("Waiting   : %d\n", wait[i]);
        printf("Turnaround: %d\n", turnaround[i]);
        printf("Completion: %d\n", completion[i]);
    }
    
    // print results in chart
    char chart[n][completion[n-1]];
    for(int i=0; i<n; i++) {
        if(processes[i][0] == '\0') {
            continue;
        }
        
        printf("\n%s\t", processes[i]);
        
        // use '#' for running, '_' for waiting
        for(int j=0; j<completion[n-1]; j++) {
            if(j >= response[i] + arrival[i] && j < completion[i]) {
                chart[i][j] = '#';
                printf("#");
            }
            else if(j < completion[i]) {
                chart[i][j] = '_';
                printf("_");
            }
            else {
                chart[i][j] = ' ';
                printf(" ");
            }
        }
    }
}

void print_stats() {
    // use '#' for running, '_' for waiting

    // calculate average waiting time

    // calculate average response time

    // calculate throughput over 10 cycles

    // print calculations below chart
}

int main() {
    // read in file
    // store contents
    // <name>   <burst (int)>   <arrival (int)>

    char processes[4][10] = {"Mouse", "Paint", "NN   ", "Word "};
    int burst[4] = {3, 2, 15, 8};
    int arrival[4] = {1, 2, 0, 4};

    for(int i=0; i<4; i++) {
        if(processes[i][0] == '\0') {
            continue;
        }
        printf("%s\t%d\t%d\n", processes[i], burst[i], arrival[i]);
    }

    // FIFO scheduling
    fifo_scheduler(4, processes, burst, arrival);

    // SJF scheduling

    // round-robin scheduling (Q=1)

    return 0;
}