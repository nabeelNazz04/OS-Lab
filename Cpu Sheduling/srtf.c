#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>

struct process {
    int pid;
    int at;  // Arrival Time
    int bt;  // Burst Time
    int ct;  // Completion Time
    int wt;  // Waiting Time
    int tat; // Turnaround Time
    int rt;  // Remaining Time
    int st;  // Start Time
} p[10], temp[10];

// Function to find the process with shortest remaining time
int find_shortest_job(int n, int current_time) {
    int shortest_job_index = -1;
    int min_remaining_time = INT_MAX;
    
    for (int i = 0; i < n; i++) {
        if (p[i].at <= current_time && p[i].rt > 0) {
            if (p[i].rt < min_remaining_time) {
                min_remaining_time = p[i].rt;
                shortest_job_index = i;
            }
        }
    }
    
    return shortest_job_index;
}

// Function to execute processes using SRTF algorithm
void srtf_execute(int n) {
    int completed = 0;
    int current_time = 0;
    int prev = -1;
    int is_first_process = 1;
    
    // Make a copy of processes for execution
    for (int i = 0; i < n; i++) {
        temp[i] = p[i];
    }
    
    printf("\nGantt Chart:\n");
    printf("+");
    
    // Continue until all processes are completed
    while (completed != n) {
        int idx = find_shortest_job(n, current_time);
        
        // If no process is available, move to the next arrival
        if (idx == -1) {
            int next_arrival_time = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (p[i].at > current_time && p[i].rt > 0 && p[i].at < next_arrival_time) {
                    next_arrival_time = p[i].at;
                }
            }
            
            if (prev != -1) {
                printf("-----------+");
                printf("\n| P%d        ", p[prev].pid);
            } else if (!is_first_process) {
                printf("-----------+");
                printf("\n| idle      ");
            } else {
                printf("-----------+");
                printf("\n| idle      ");
                is_first_process = 0;
            }
            
            prev = -1;
            current_time = next_arrival_time;
        } else {
            // If current process is different from previous, update Gantt chart
            if (idx != prev) {
                if (prev != -1 || !is_first_process) {
                    printf("-----------+");
                    if (prev != -1) {
                        printf("\n| P%d        ", p[prev].pid);
                    } else {
                        printf("\n| idle      ");
                    }
                } else {
                    printf("-----------+");
                    printf("\n| P%d        ", p[idx].pid);
                    is_first_process = 0;
                }
                
                // Record start time if process is starting for the first time
                if (p[idx].rt == p[idx].bt) {
                    p[idx].st = current_time;
                }
                
                prev = idx;
            }
            
            // Execute the process for 1 time unit
            p[idx].rt--;
            current_time++;
            
            // If process is completed
            if (p[idx].rt == 0) {
                completed++;
                p[idx].ct = current_time;
                p[idx].tat = p[idx].ct - p[idx].at;
                p[idx].wt = p[idx].tat - p[idx].bt;
            }
        }
    }
    
    // Complete the Gantt chart
    printf("-----------+");
    printf("\n| P%d        |\n", p[prev].pid);
    printf("+");
    
    for (int i = 0; i < n + 1; i++) {
        printf("-----------+");
    }
    
    printf("\n");
}

void calc_avg(int n) {
    float wt = 0, tat = 0;
    for (int i = 0; i < n; i++) {
        tat += p[i].tat;
        wt += p[i].wt;
    }
    float avg_wt = wt / n;
    float avg_tat = tat / n;
    printf("\nAverage Turnaround Time: %.2f\nAverage Waiting Time: %.2f\n", avg_tat, avg_wt);
}

void displaytable(int n) {
    printf("\nProcess Details:\n");
    printf("PID\tAT\tBT\tST\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
               p[i].pid, p[i].at, p[i].bt, p[i].st, p[i].ct, p[i].tat, p[i].wt);
    }
}

void sort_by_pid(int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].pid > p[j + 1].pid) {
                struct process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

void sort_by_arrival(int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at) {
                struct process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    for (int i = 0; i < n; i++) {
        printf("Enter the process id, arrival time and burst time of process %d: ", i + 1);
        scanf("%d%d%d", &(p[i].pid), &(p[i].at), &(p[i].bt));
        p[i].rt = p[i].bt; // Initialize remaining time
    }
    
    // Sort processes by arrival time
    sort_by_arrival(n);
    
    // Execute SRTF
    srtf_execute(n);
    
    // Sort by PID for final display
    sort_by_pid(n);
    
    // Display results
    calc_avg(n);
    displaytable(n);
    
    return 0;
}