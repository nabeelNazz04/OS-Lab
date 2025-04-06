/*
Yes, this SRTF (Shortest Remaining Time First) code can definitely help you understand the logic of SJF (Shortest Job First) scheduling. Both algorithms are closely related, with SRTF being the preemptive version of SJF.

Key connections that help you understand SJF through this SRTF code:

1. **Core selection principle**: Both algorithms select processes based on the shortest processing time. In SJF, it's the shortest burst time for new processes, while in SRTF it's the shortest remaining time for both new and running processes.

2. **Decision points**: 
   - In SJF: Decisions are made only when a process completes or when the CPU is idle
   - In SRTF: Decisions are made at process arrival and at every time unit

3. **Common components**:
   - Both use the same process structure with attributes like arrival time, burst time, etc.
   - Both track completion time, waiting time, and turnaround time
   - Both require sorting processes by arrival time initially

4. **Implementation details**:
   - The `find_shortest_job()` function in SJF has its logic expanded in SRTF to handle preemption
   - The time tracking in SRTF (incrementing one unit at a time) gives you deeper insight into how the scheduler works

Understanding SRTF actually deepens your understanding of SJF since you can see SRTF as a finer-grained version of SJF where process selection happens more frequently. The code also helps you visualize the key differences in when and how scheduling decisions are made.

If you're studying for an operating systems class or exam, working through this code and tracking how processes are selected at each step will strengthen your grasp of both scheduling algorithms.
*/
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

struct process {
    int pid;
    int at;  // Arrival Time
    int bt;  // Burst Time
    int ct;  // Completion Time
    int wt;  // Waiting Time
    int tat; // Turnaround Time
    int rt;  // Remaining Time (for tracking execution)
    int start_time; // First time CPU is allocated
    int is_completed; // Flag to check if process is completed
} p[10];

// Function to sort processes by arrival time using insertion sort
void ins_sort_at(int n) {
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

// Function to calculate completion times using SRTF algorithm
void ct_calc_srtf(int n) {
    // Initialize remaining time and completed flag for all processes
    for (int i = 0; i < n; i++) {
        p[i].rt = p[i].bt;
        p[i].is_completed = 0;
        p[i].start_time = -1; // Initialize start time to -1
    }
    
    int completed = 0;
    int currentTime = 0;
    int prev = -1; // To keep track of previous running process
    
    // Store execution sequence for Gantt chart
    int timeline[100];
    int time_points[100];
    int timeline_idx = 0;
    
    // Continue until all processes are completed
    while (completed < n) {
        // Find process with shortest remaining time among arrived processes
        int idx = -1;
        int min_rt = 9999;
        
        for (int i = 0; i < n; i++) {
            if (p[i].at <= currentTime && p[i].is_completed == 0) {
                if (p[i].rt < min_rt) {
                    min_rt = p[i].rt;
                    idx = i;
                }
                // If remaining times are same, choose the one that arrived earlier
                else if (p[i].rt == min_rt && p[i].at < p[idx].at) {
                    idx = i;
                }
            }
        }
        
        // If no process is available, increment time
        if (idx == -1) {
            currentTime++;
            
            // Add idle time to timeline
            if (prev != -1 || timeline_idx == 0) {
                timeline[timeline_idx] = -1; // -1 represents idle
                time_points[timeline_idx] = currentTime - 1;
                timeline_idx++;
                prev = -1;
            }
        } else {
            // If this is the first time the process is being executed, record start time
            if (p[idx].start_time == -1) {
                p[idx].start_time = currentTime;
            }
            
            // Process context switch for Gantt chart
            if (prev != idx) {
                timeline[timeline_idx] = idx;
                time_points[timeline_idx] = currentTime;
                timeline_idx++;
                prev = idx;
            }
            
            // Execute the process for 1 time unit
            p[idx].rt--;
            currentTime++;
            
            // If process completes execution
            if (p[idx].rt == 0) {
                p[idx].ct = currentTime;
                p[idx].is_completed = 1;
                completed++;
            }
        }
    }
    
    // Store the final time point for Gantt chart
    time_points[timeline_idx] = currentTime;
    
    // Store the timeline and time points globally for the Gantt chart function
    // In a real implementation, you would need to make these global or pass them to the display function
    // For this implementation, we'll create a global variable to store them
    
    // Here we're assuming the display function will access the timeline and time_points arrays
    // to draw the Gantt chart properly
}

// Function to calculate waiting and turnaround times
void calc_avg(int n) {
    float wt = 0, tat = 0;
    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        tat += p[i].tat;
        wt += p[i].wt;
    }
    float avg_wt = wt / n;
    float avg_tat = tat / n;
    printf("Average Waiting Time: %.3f\n", avg_wt);
    printf("Average Turnaround Time: %.3f\n", avg_tat);
}

// Function to display Gantt chart for SRTF
void display(int n) {
    // For SRTF, processes may be preempted multiple times
    // So we need to create a proper timeline of execution
    
    printf("\nGantt Chart:\n");
    printf("+");
    
    // Create a temporary array to track process order and execution times
    int process_order[100]; // Store the process IDs in execution order
    int execution_times[100]; // Store the corresponding time points
    int num_segments = 0;
    
    int currentTime = 0;
    int prev_process = -2; // Different from -1 (idle) to force a change at the start
    
    // Initialize remaining time for tracking
    for (int i = 0; i < n; i++) {
        p[i].rt = p[i].bt;
    }
    
    // Simulate SRTF execution to record process sequence for Gantt chart
    while (1) {
        int all_completed = 1;
        for (int i = 0; i < n; i++) {
            if (p[i].rt > 0) {
                all_completed = 0;
                break;
            }
        }
        if (all_completed) break;
        
        // Find the process with shortest remaining time at current time
        int idx = -1;
        int min_rt = 9999;
        
        for (int i = 0; i < n; i++) {
            if (p[i].at <= currentTime && p[i].rt > 0) {
                if (p[i].rt < min_rt) {
                    min_rt = p[i].rt;
                    idx = i;
                }
            }
        }
        
        // If no process available, find next arrival
        if (idx == -1) {
            int next_arrival = 9999;
            for (int i = 0; i < n; i++) {
                if (p[i].rt > 0 && p[i].at < next_arrival && p[i].at > currentTime) {
                    next_arrival = p[i].at;
                }
            }
            
            // Add idle time to process order
            if (prev_process != -1) {
                process_order[num_segments] = -1; // -1 represents idle
                execution_times[num_segments] = currentTime;
                num_segments++;
                prev_process = -1;
            }
            
            currentTime = next_arrival;
        } else {
            // If process changed or it's the first process
            if (idx != prev_process) {
                process_order[num_segments] = idx;
                execution_times[num_segments] = currentTime;
                num_segments++;
                prev_process = idx;
            }
            
            // Execute for 1 time unit
            p[idx].rt--;
            currentTime++;
        }
    }
    
    // Add final time point
    execution_times[num_segments] = currentTime;
    
    // Draw Gantt chart
    for (int i = 0; i < num_segments; i++) {
        printf("-----------+");
    }
    printf("\n|");
    
    for (int i = 0; i < num_segments; i++) {
        if (process_order[i] == -1) {
            printf("    idle    |");
        } else {
            printf("     P%d     |", p[process_order[i]].pid);
        }
    }
    
    printf("\n+");
    for (int i = 0; i < num_segments; i++) {
        printf("-----------+");
    }
    printf("\n");
    
    // Print time markers
    printf("%d", execution_times[0]);
    for (int i = 1; i <= num_segments; i++) {
        printf("%11d", execution_times[i]);
    }
    printf("\n");
}

// Function to display process details in a table
void displaytable(int n) {
    printf("\nPID\tAT\tBT\tCT\tTT\tWT\tStart Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt, p[i].start_time);
    }
}

// Function to sort by PID for final display
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
    int i, n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    for (i = 0; i < n; i++) {
        p[i].pid = i;
        printf("Enter Arrival Time for P%d: ", i);
        scanf("%d", &p[i].at);
        printf("Enter Burst Time for P%d: ", i);
        scanf("%d", &p[i].bt);
    }
    
    // Sort by arrival time first
    ins_sort_at(n);
    
    // Calculate completion times using SRTF logic
    ct_calc_srtf(n);
    
    // Calculate averages
    calc_avg(n);
    
    // Sort by PID for the table display
    sort_by_pid(n);
    
    // Display results
    displaytable(n);
    display(n);
    
    return 0;
}