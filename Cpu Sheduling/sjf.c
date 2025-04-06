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
} p[10];
// Find the process that completed at position 'pos'
int find_next_completion(int n, int pos) {
    int min_ct = 9999;
    int idx = -1;
    int count = 0;
    
    for (int i = 0; i < n; i++) {
        if (p[i].rt == 0) {
            count++;
            if (count == pos + 1 && p[i].ct < min_ct) {
                min_ct = p[i].ct;
                idx = i;
            }
        }
    }
    
    return idx;
}
// Find the process with shortest burst time among available processes
int find_shortest_job(int n, int current_time) {
    int shortest_idx = -1;
    int min_burst = 9999;
    
    for (int i = 0; i < n; i++) {
        if (p[i].at <= current_time && p[i].rt > 0) {
            if (p[i].bt < min_burst) {
                min_burst = p[i].bt;
                shortest_idx = i;
            }
        }
    }
    
    return shortest_idx;
}

void sjf_execute(int n) {
    int completed = 0;
    int current_time = 0;
    int total_idle_time = 0;
    
    printf("\nGantt Chart:\n");
    printf("+");
    for (int i = 0; i < n + (p[0].at > 0 ? 1 : 0); i++) {
        printf("-----------+");
    }
    printf("\n|");
    
    // If first process arrives after time 0, show idle time
    if (p[0].at > 0) {
        printf("    idle    |");
        total_idle_time += p[0].at;
        current_time = p[0].at;
    }
    
    while (completed < n) {
        int idx = find_shortest_job(n, current_time);
        
        // If no process is available, find next arrival
        if (idx == -1) {
            int next_arrival = 9999;
            for (int i = 0; i < n; i++) {
                if (p[i].rt > 0 && p[i].at < next_arrival && p[i].at > current_time) {
                    next_arrival = p[i].at;
                }
            }
            
            printf("    idle    |");
            total_idle_time += (next_arrival - current_time);
            current_time = next_arrival;
        } else {
            // Execute the selected process
            printf("     P%d     |", p[idx].pid);
            current_time += p[idx].bt;
            p[idx].ct = current_time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            p[idx].rt = 0;
            completed++;
        }
    }
    
    printf("\n+");
    for (int i = 0; i < n + (p[0].at > 0 ? 1 : 0); i++) {
        printf("-----------+");
    }
    
    printf("\n");
    
    // Print time markers
    printf("0");
    if (p[0].at > 0) {
        printf("%11d", p[0].at);
    }
    
    for (int i = 0; i < n; i++) {
        int idx = find_next_completion(n, i);
        if (idx != -1) {
            printf("%11d", p[idx].ct);
        }
    }
    printf("\n");
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
    printf("\nPID\tAT\tBT\tCT\tTT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", 
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
}

// Sort processes by arrival time
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

// Sort processes by completion time for Gantt chart
void sort_by_completion(int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].ct > p[j + 1].ct) {
                struct process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

// Sort processes by process ID
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

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    for (int i = 0; i < n; i++) {
        p[i].pid = i;
        printf("Enter Arrival Time: ");
        scanf("%d", &p[i].at);
        printf("Enter Burst Time: ");
        scanf("%d", &p[i].bt);
        p[i].rt = p[i].bt;  // Initialize remaining time
    }
    
    // Sort processes by arrival time
    sort_by_arrival(n);
    
    // Execute non-preemptive SJF algorithm
    sjf_execute(n);
    
    // Calculate averages and display results
    calc_avg(n);
    
    // Sort by PID for display
    sort_by_pid(n);
    displaytable(n);
    
    return 0;
}