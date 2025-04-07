#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

struct process {
    int pid;
    int at;  // Arrival Time
    int bt;  // Burst Time
    int rt;  // Remaining Time
    int ct;  // Completion Time
    int wt;  // Waiting Time
    int tat; // Turnaround Time
    int started; // Flag to check if process has started execution
} p[10];

// Gantt chart information - Increased size to handle more context switches
#define MAX_CHART_SIZE 1000
struct gantt_chart {
    int pid;     // Process ID (-1 for idle)
    int start;   // Start time
    int end;     // End time
} chart[MAX_CHART_SIZE];    // Increased size significantly

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

// Function to calculate completion times using Round Robin algorithm
int ct_calc_rr(int n, int quantum) {
    // Create a copy for remaining time
    for (int i = 0; i < n; i++) {
        p[i].rt = p[i].bt;
        p[i].started = 0;
    }
    
    int chart_index = 0;
    int current_time = 0;
    int completed = 0;
    int queue[100];  // Increased size for safety
    int front = 0, rear = 0;
    
    // Check if any process has arrived at time 0
    for (int i = 0; i < n; i++) {
        if (p[i].at == 0) {
            queue[rear] = i;
            rear = (rear + 1) % 100;  // Using modulo with the array size
            p[i].started = 1;
        }
    }
    
    // If no processes at time 0, add idle time
    if (front == rear) {
        // Find the first process to arrive
        int earliest = 999999;
        for (int i = 0; i < n; i++) {
            if (p[i].at < earliest) {
                earliest = p[i].at;
            }
        }
        
        // Safety check for chart array bounds
        if (chart_index < MAX_CHART_SIZE) {
            chart[chart_index].pid = -1;  // -1 represents idle
            chart[chart_index].start = 0;
            chart[chart_index].end = earliest;
            chart_index++;
        }
        
        current_time = earliest;
        
        // Now add the first process(es) to the queue
        for (int i = 0; i < n; i++) {
            if (p[i].at == earliest) {
                queue[rear] = i;
                rear = (rear + 1) % 100;  // Using modulo with the array size
                p[i].started = 1;
            }
        }
    }
    
    // Main Round Robin loop
    while (completed != n) {
        // If queue is empty but processes remain
        if (front == rear && completed != n) {
            int next_arrival = 999999;
            int next_process = -1;
            
            // Find the next process to arrive
            for (int i = 0; i < n; i++) {
                if (p[i].rt > 0 && p[i].at > current_time && p[i].at < next_arrival) {
                    next_arrival = p[i].at;
                    next_process = i;
                }
            }
            
            // Safety check for chart array bounds
            if (chart_index < MAX_CHART_SIZE) {
                // Add idle time to Gantt chart
                chart[chart_index].pid = -1;
                chart[chart_index].start = current_time;
                chart[chart_index].end = next_arrival;
                chart_index++;
            }
            
            current_time = next_arrival;
            
            // Add all processes that arrive at this time to the queue
            for (int i = 0; i < n; i++) {
                if (p[i].rt > 0 && p[i].at <= current_time && !p[i].started) {
                    queue[rear] = i;
                    rear = (rear + 1) % 100;  // Using modulo with the array size
                    p[i].started = 1;
                }
            }
            continue;
        }
        
        // Get the next process from the queue
        int curr_process = queue[front];
        front = (front + 1) % 100;  // Using modulo with the array size
        
        // Calculate execution time for this round
        int exec_time = (p[curr_process].rt < quantum) ? p[curr_process].rt : quantum;
        
        // Safety check for chart array bounds
        if (chart_index < MAX_CHART_SIZE) {
            // Add to Gantt chart
            chart[chart_index].pid = p[curr_process].pid;
            chart[chart_index].start = current_time;
            chart[chart_index].end = current_time + exec_time;
            chart_index++;
        }
        
        // Update process remaining time
        p[curr_process].rt -= exec_time;
        current_time += exec_time;
        
        // Check for newly arrived processes during this time slice
        for (int i = 0; i < n; i++) {
            if (p[i].rt > 0 && p[i].at <= current_time && !p[i].started) {
                queue[rear] = i;
                rear = (rear + 1) % 100;  // Using modulo with the array size
                p[i].started = 1;
            }
        }
        
        // If the current process is not finished, put it back in the queue
        if (p[curr_process].rt > 0) {
            queue[rear] = curr_process;
            rear = (rear + 1) % 100;  // Using modulo with the array size
        } else {
            // Process is completed
            p[curr_process].ct = current_time;
            completed++;
        }
    }
    
    // Return the actual number of chart entries
    return chart_index;
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

// Function to display Gantt chart based on execution order
void display(int chart_size) {
    // Limit chart size for display
    int display_size = chart_size;
    if (display_size > MAX_CHART_SIZE) {
        display_size = MAX_CHART_SIZE;
        printf("Warning: Gantt chart has been truncated due to large size.\n");
    }
    
    // Draw Gantt chart header
    printf("\n");
    printf("+");
    for (int i = 0; i < display_size; i++) {
        printf("-----------+");
    }
    printf("\n|");
    
    // Fill in the processes
    for (int i = 0; i < display_size; i++) {
        if (chart[i].pid == -1) {
            printf("    idle    |");
        } else {
            printf("     P%d     |", chart[i].pid);
        }
    }
    
    // Draw Gantt chart footer
    printf("\n+");
    for (int i = 0; i < display_size; i++) {
        printf("-----------+");
    }
    printf("\n");
    
    // Print time markers
    printf("%d", chart[0].start);
    for (int i = 0; i < display_size; i++) {
        printf("%11d", chart[i].end);
    }
    printf("\n");
}

// Function to display process details in a table
void displaytable(int n) {
    printf("\nPID\tAT\tBT\tCT\tTT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", 
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
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
    int i, n, quantum;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    for (i = 0; i < n; i++) {
        p[i].pid = i;
        printf("Enter Arrival Time for P%d: ", i);
        scanf("%d", &p[i].at);
        printf("Enter Burst Time for P%d: ", i);
        scanf("%d", &p[i].bt);
    }
    
    printf("Enter Time Quantum: ");
    scanf("%d", &quantum);
    
    // Sort by arrival time
    ins_sort_at(n);
    
    // Calculate completion times using Round Robin logic
    int chart_size = ct_calc_rr(n, quantum);
    
    // Calculate averages
    calc_avg(n);
    
    // Make a copy of the current process array for display
    struct process proc_copy[10];
    for (i = 0; i < n; i++) {
        proc_copy[i] = p[i];
    }
    
    // Sort by PID for the table display
    sort_by_pid(n);
    
    // Display results in PID order
    displaytable(n);
    
    // Display Gantt chart
    display(chart_size);
    
    return 0;
}