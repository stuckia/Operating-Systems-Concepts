#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    char proc_name[32];     // hold process name
    char proc_chart[256];   // hold output chart for process
    int burst;              // burst time
    int arrival;            // arrival time
    int start;              // start time
    int finish;             // finish time
    int wait;               // waiting time
    int remaining;          // remaining time
    int response;           // response time
} Process;

// Name: Abby Stucki
// Date: 11/10/2024
// Description: Initialize/reset process fields
void reset_procs(int n, Process procs[]) {
    for(int i=0; i<n; i++) {
        strcpy(procs[i].proc_chart,"");
        procs[i].start = 0;
        procs[i].finish = 0;
        procs[i].wait = 0;
        procs[i].remaining = 0;
        procs[i].response = 0;
    }
}

// Name: Abby Stucki
// Date: 11/12/2024
// Description: Return last index of process chart
int get_last_index(Process* proc) {
    int i=0;
    while(proc->proc_chart[i] != '\0') {
        i++;
    }
    return i;
}

// Name: Abby Stucki
// Date: 11/10/2024
// Description: Print fields for analysis
void print_vars(int n, Process procs[], int proc_complete) {
    int total_wait = 0;
    int total_response = 0;
    float avg_wait = 0.0;
    float avg_response = 0.0;
    
    for(int i=0; i<n; i++) {
        printf("\n%-10s: ", procs[i].proc_name);
        for(int j=0; j<get_last_index(&procs[i]); j++) {
            printf("%c", procs[i].proc_chart[j]);
        }
        total_wait += procs[i].wait;
        total_response += procs[i].response;
    }

    avg_wait = (float)total_wait / n;
    avg_response = (float)total_response / n;
    printf("\nAverage wait time: %.2f\n", avg_wait);
    printf("Average response time: %.2f\n", avg_response);
    printf("Throughput over 10 cycles: %d\n", proc_complete);
}

// Name: Abby Stucki
// Date: 11/10/2024
// Description: FIFO scheduling
void fifo_scheduler(int n, Process procs[]) {
    int cycle = 0;
    int proc_complete = 0;
    
    printf("\nFIFO Scheduling:\n");

    for(int i=0; i<n; i++) {
        for(int j=0; j<1; j++) {
            procs[i].wait += procs[j].burst;
        }

        procs[i].wait -= procs[i].burst;
        procs[i].response = procs[i].wait;
        cycle += procs[i].burst;

        if(cycle <= 10) {
            proc_complete++;
        }
    }

    // create chart depiction of process timeline
    for(int i=0; i<n; i++) {
        cycle = 0;
        for(int j=0; j<=i; j++) {
            for(int k=0; k<procs[j].burst; k++) {
                if(j==i) {
                    procs[i].proc_chart[cycle] = '#';
                }
                else if(cycle >= procs[i].arrival) {
                    procs[i].proc_chart[cycle] = '_';
                }
                else {
                    procs[i].proc_chart[cycle] = ' ';
                }
                cycle++;
            }
        }
    }

    print_vars(n, procs, proc_complete);
    reset_procs(n, procs);
}

// Name: Abby Stucki
// Date: 11/12/2024
// Description: SJF scheduling with preemption
void srtf_scheduler(int n, Process procs[]) {
    int cycle = 0;
    int proc_complete = 0;
    int total_proc_complete = 0;

    printf("\nSRTF Scheduling:\n");

    while(total_proc_complete < n) {
        int index = 0;
        Process* shortest = NULL;

        for(int i=1; i<n; i++) {
            if((procs[i].remaining > 0) && (procs[i].arrival < cycle)) {
                if((procs[i].remaining < shortest->remaining) || (shortest == NULL)) {
                    index = i;
                    shortest = & procs[index];
                }
            }
        }

        if(shortest) {
            if(shortest->burst == shortest->remaining) {
                shortest->start = cycle;
                shortest->response = cycle - (shortest->arrival);
            }

            shortest->remaining--;
            shortest->proc_chart[get_last_index(shortest)] = '#';

            if(shortest->remaining == 0) {
                total_proc_complete++;
                if(cycle <= 10) {
                    proc_complete++;
                }
            }
        }
        
        for(int i=0; i<n; i++) {
            if(index != i) {
                if(procs[i].arrival > cycle) {
                    procs[i].proc_chart[get_last_index(&procs[i])] = ' ';
                }
                else if(procs[i].remaining > 0) {
                    procs[i].proc_chart[get_last_index(&procs[i])] = '_';
                    if(procs[i].remaining > 0) {
                        procs[i].wait++;
                    }
                }
            }
        }

        cycle++;
    }

    print_vars(n, procs, proc_complete);
    reset_procs;
}

// Name: Abby Stucki
// Date: 11/15/2024
// Description: Round-robin scheduling with q=1
void rr_scheduler(int n, Process procs[]) {
    Process* queue[5];
    bool started[5] = {false};
    int cycle = 0;
    int proc_complete = 0;
    int total_proc_complete = 0;
    int procs_started = 0;
    int index = 0;
    int end = 0;
    int queue_size = 0;

    printf("\nRound Robin Scheduling:\n");

    Process* curr_proc = NULL;
    while(total_proc_complete < n) {
        if(procs_started < n) {
            for(int i=0; i<n; i++) {
                if((procs[i].arrival <= cycle) && (!started[i])) {
                    queue[end] = &procs[i];
                    end = (end + 1) % n;
                    procs_started++;
                    started[i] = true;
                    queue_size++;
                }
                else if(!started[i]) {
                    procs[i].proc_chart[get_last_index(&procs[i])] = ' ';
                }
            }
        }

        if(curr_proc && curr_proc->remaining > 0) {
            queue[end] = curr_proc;
            end = (end + 1) % n;
            queue_size++;
        }

        if(queue_size > 0) {
            curr_proc = queue[index];
            index = (index + 1) % n;
            queue_size--;

            if(curr_proc->remaining == curr_proc->burst) {
                curr_proc->start = cycle;
                curr_proc->response = cycle - curr_proc->arrival;
            }

            curr_proc->remaining--;
            curr_proc->proc_chart[get_last_index(curr_proc)] = '#';

            if(curr_proc->remaining == 0) {
                total_proc_complete++;
                if(cycle <= 10) {
                    proc_complete++;
                }
            }

            for(int k=index; k != end; k = (k+1) % n) {
                queue[k]->proc_chart[get_last_index(queue[k])] = '_';
                queue[k]->wait++;
            }
        }

        cycle++;
    }

    print_vars(n, procs, proc_complete);
    reset_procs(n, procs);
}

// Name: Abby Stucki
// Date: 11/10/2024
// Description: Read from file, begin the scheduler
int main() {
    int n = 0;
    Process procs[5];

    // open file, ensure file is valid
    FILE *proc_file = fopen("processes.txt", "r");
    if(proc_file == NULL) {
        printf("File cannot be opened.");
        return 1;
    }

    // retrieve data from process file
    // <name>   <burst (int)>   <arrival (int)>
    while(fscanf(proc_file, "%s %d %d", procs[n].proc_name, &procs[n].burst, &procs[n].arrival)==3) {
        n++;
    }
    fclose(proc_file);

    reset_procs(n, procs);

    // bubble sort processes
    Process temp;
    for(int i=0; i<n-1; i++) {
        for(int j=0; j<n-1; j++) {
            if(procs[j].arrival > procs[j+1].arrival) {
                temp = procs[j];
                procs[j] = procs[j+1];
                procs[j+1] = temp;
            }
        }
    }

    // FIFO scheduling
    fifo_scheduler(n, procs);

    // SJF scheduling
    srtf_scheduler(n, procs);

    // round-robin scheduling (Q=1)
    rr_scheduler(n, procs);

    return 0;
}