#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

struct process {
    int pid;
    int at;  // Arrival Time
    int bt;  // Burst Time
    int ct;  // Completion Time
    int wt;  // Waiting Time
    int tat; // Turnaround Time (also shown as TT in your output)
    int rt;  // Remaining Time
    int start_time; // First time process is executed
} p[10];

// Find the process with shortest remaining time among available processes
int find_shortest_remaining_time(int n, int current_time) {
    int shortest_idx = -1;
    int min_remaining = 9999;
    
    for (int i = 0; i < n; i++) {
        if (p[i].at <= current_time && p[i].rt > 0) {
            if (p[i].rt < min_remaining) {
                min_remaining = p[i].rt;
                shortest_idx = i;
            }
        }
    }
    
    return shortest_idx;
}

void srtf_execute(int n) {
    int completed = 0;
    int current_time = 0;
    int prev = -1;
    int total_idle_time = 0;
    int context_switches = 0;
    
    // Initialize start_time for each process to -1 (not started yet)
    for (int i = 0; i < n; i++) {
        p[i].start_time = -1;
    }
    
    printf("\nGantt Chart:\n");
    printf("+");
    // We don't know how many segments we'll have in advance with SRTF
    // So we'll build the Gantt chart incrementally
    
    // Store time points and process IDs for Gantt chart
    int time_points[100];  // Assuming no more than 100 context switches
    int proc_ids[100];
    int chart_idx = 0;
    
    time_points[chart_idx] = 0;
    
    // If first process arrives after time 0, show idle time
    if (p[0].at > 0) {
        proc_ids[chart_idx++] = -1;  // -1 represents idle
        time_points[chart_idx] = p[0].at;
        total_idle_time += p[0].at;
        current_time = p[0].at;
    }
    
    while (completed < n) {
        int idx = find_shortest_remaining_time(n, current_time);
        
        // If no process is available, find next arrival
        if (idx == -1) {
            int next_arrival = 9999;
            for (int i = 0; i < n; i++) {
                if (p[i].rt > 0 && p[i].at < next_arrival && p[i].at > current_time) {
                    next_arrival = p[i].at;
                }
            }
            
            proc_ids[chart_idx++] = -1;  // -1 represents idle
            time_points[chart_idx] = next_arrival;
            total_idle_time += (next_arrival - current_time);
            current_time = next_arrival;
        } else {
            // If this is the first time the process is executing, set its start time
            if (p[idx].start_time == -1) {
                p[idx].start_time = current_time;
            }
            
            // Check if there's a context switch
            if (prev != idx && prev != -1) {
                context_switches++;
            }
            
            // If the current process is different from the previous one, update the Gantt chart
            if (prev != idx) {
                proc_ids[chart_idx++] = idx;
                time_points[chart_idx] = current_time + 1;
                prev = idx;
            } else {
                // Just update the end time of the current segment
                time_points[chart_idx] = current_time + 1;
            }
            
            // Execute for 1 time unit
            current_time++;
            p[idx].rt--;
            
            // If the process has completed
            if (p[idx].rt == 0) {
                completed++;
                p[idx].ct = current_time;
                p[idx].tat = p[idx].ct - p[idx].at;
                p[idx].wt = p[idx].tat - p[idx].bt;
            }
        }
    }
    
    // Now draw the Gantt chart based on the collected data
    for (int i = 0; i < chart_idx; i++) {
        printf("-----------+");
    }
    printf("\n|");
    
    for (int i = 0; i < chart_idx; i++) {
        if (proc_ids[i] == -1) {
            printf("    idle    |");
        } else {
            printf("     P%d     |", p[proc_ids[i]].pid);
        }
    }
    
    printf("\n+");
    for (int i = 0; i < chart_idx; i++) {
        printf("-----------+");
    }
    
    printf("\n");
    
    // Print time markers
    for (int i = 0; i <= chart_idx; i++) {
        printf("%-11d", time_points[i]);
    }
    
    printf("\n\nNumber of Context Switches: %d\n", context_switches);
}

void calc_avg(int n) {
    float total_wt = 0, total_tat = 0;
    for (int i = 0; i < n; i++) {
        total_wt += p[i].wt;
        total_tat += p[i].tat;
    }
    
    printf("Average Waiting Time: %.3f\n", total_wt / n);
    printf("Average Turnaround Time: %.3f\n", total_tat / n);
}

void displaytable(int n) {
    printf("\nPID\tAT\tBT\tCT\tTT\tWT\tStart Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt, p[i].start_time);
    }
}

// Sort processes by arrival time using insertion sort
void sort_by_arrival(int n) {
    int i, j;
    for (i = 1; i < n; i++) {
        struct process key = p[i];
        j = i - 1;
        while (j >= 0 && p[j].at > key.at) {
            p[j + 1] = p[j];
            j--;
        }
        p[j + 1] = key;
    }
}

// Sort processes by completion time using insertion sort
void sort_by_completion(int n) {
    int i, j;
    for (i = 1; i < n; i++) {
        struct process key = p[i];
        j = i - 1;
        while (j >= 0 && p[j].ct > key.ct) {
            p[j + 1] = p[j];
            j--;
        }
        p[j + 1] = key;
    }
}

// Sort processes by process ID using insertion sort
void sort_by_pid(int n) {
    int i, j;
    for (i = 1; i < n; i++) {
        struct process key = p[i];
        j = i - 1;
        while (j >= 0 && p[j].pid > key.pid) {
            p[j + 1] = p[j];
            j--;
        }
        p[j + 1] = key;
    }
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    for (int i = 0; i < n; i++) {
        p[i].pid = i;
        printf("Enter Arrival Time for P%d: ", i);
        scanf("%d", &p[i].at);
        printf("Enter Burst Time for P%d: ", i);
        scanf("%d", &p[i].bt);
        p[i].rt = p[i].bt;  // Initialize remaining time
    }
    
    // Sort processes by arrival time
    sort_by_arrival(n);
    
    // Execute preemptive SRTF algorithm
    srtf_execute(n);
    
    // Calculate averages and display results
    calc_avg(n);
    
    // Sort by PID for display
    sort_by_pid(n);
    displaytable(n);
    
    return 0;
}