#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

struct process {
    int pid;
    int at;  // Arrival Time
    int bt;  // Burst Time
    int pr;  // Priority (lower number means higher priority)
    int ct;  // Completion Time
    int wt;  // Waiting Time
    int tat; // Turnaround Time
    int completed; // Flag to mark if process is completed
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

// Function to calculate completion times using non-preemptive priority scheduling
void ct_calc_priority(int n) {
    int currentTime = 0;
    int completed = 0;
    int i;
    
    // Initialize completion status
    for (i = 0; i < n; i++) {
        p[i].completed = 0;
    }
    
    // Continue until all processes are completed
    while (completed < n) {
        int highest_priority = -1;
        int priority_value = 999999; // High value for minimum priority
        
        // Find the process with highest priority among the arrived and not completed processes
        for (i = 0; i < n; i++) {
            if (p[i].at <= currentTime && p[i].completed == 0 && p[i].pr < priority_value) {
                priority_value = p[i].pr;
                highest_priority = i;
            }
        }
        
        // Execute the process with highest priority
        i = highest_priority;
        currentTime += p[i].bt;
        p[i].ct = currentTime;
        p[i].completed = 1;
        completed++;
    }
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

// Function to get process execution order
void get_exec_order(int n, int exec_order[]) {
    struct process temp[10];
    int i;
    
    // Create a temporary copy of processes
    for (i = 0; i < n; i++) {
        temp[i] = p[i];
    }
    
    // Sort by completion time
    for (i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (temp[j].ct > temp[j + 1].ct) {
                struct process t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }
    
    // Get execution order by PID
    for (i = 0; i < n; i++) {
        exec_order[i] = temp[i].pid;
    }
}

// Function to display Gantt chart based on completion order
void display(int n) {
    int exec_order[10];
    get_exec_order(n, exec_order);
    
    // Create a temporary array of processes in execution order
    struct process temp[10];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (p[j].pid == exec_order[i]) {
                temp[i] = p[j];
                break;
            }
        }
    }
    
    // Draw Gantt chart header
    printf("\n");
    printf("+");
    
    // Draw top border
    for (int i = 0; i < n; i++) {
        printf("-----------+");
    }
    printf("\n|");
    
    // Draw process names
    for (int i = 0; i < n; i++) {
        printf("     P%d     |", temp[i].pid);
    }
    
    // Draw bottom border
    printf("\n+");
    for (int i = 0; i < n; i++) {
        printf("-----------+");
    }
    printf("\n");
    
    // Print time markers
    int currentTime = 0;
    printf("%d", currentTime);
    
    for (int i = 0; i < n; i++) {
        currentTime = temp[i].ct;
        printf("%11d", currentTime);
    }
    printf("\n");
}

// Function to display process details in a table
void displaytable(int n) {
    printf("\nPID\tAT\tBT\tPR\tCT\tTT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
               p[i].pid, p[i].at, p[i].bt, p[i].pr, p[i].ct, p[i].tat, p[i].wt);
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
        printf("Enter Priority for P%d (lower number means higher priority): ", i);
        scanf("%d", &p[i].pr);
    }
    
    // Calculate completion times using Priority Scheduling
    ct_calc_priority(n);
    
    // Calculate averages
    calc_avg(n);
    
    // Get execution order
    int exec_order[10];
    get_exec_order(n, exec_order);
    
    // Display the processes in execution order
    printf("\nProcess execution order: ");
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" -> ");
        printf("P%d", exec_order[i]);
    }
    printf("\n");
    
    // Display results with processes showing execution metrics
    displaytable(n);
    display(n);
    
    // Sort by PID for the final table display
    sort_by_pid(n);
    
    // Display results sorted by PID
    printf("\nProcess table sorted by PID:\n");
    displaytable(n);
    
    return 0;
}