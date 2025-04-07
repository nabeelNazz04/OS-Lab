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

// Function to calculate completion times using FCFS algorithm
void ct_calc_fcfs(int n) {
    int currentTime = 0;
    
    for (int i = 0; i < n; i++) {
        // If arrival time is after current time, update current time
        if (p[i].at > currentTime) {
            currentTime = p[i].at;
        }
        
        // Execute the process
        currentTime += p[i].bt;
        p[i].ct = currentTime;
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

// Function to display Gantt chart based on execution order
void display(int n) {
    // Draw Gantt chart header
    printf("\n");
    printf("+");
    // Add one more cell if there's idle time at the beginning
    int extra_cell = (p[0].at > 0) ? 1 : 0;
    for (int i = 0; i < n + extra_cell; i++) {
        printf("-----------+");
    }
    printf("\n|");
    
    // Current time tracker
    int currentTime = 0;
    
    // Show idle time at beginning if needed
    if (p[0].at > 0) {
        printf("    idle    |");
        currentTime = p[0].at;
    }
    
    // Show each process in order of execution (which is the arrival order in FCFS)
    for (int i = 0; i < n; i++) {
        // If there's idle time between processes
        if (currentTime < p[i].at) {
            printf("    idle    |");
            currentTime = p[i].at;
        }
        printf("     P%d     |", p[i].pid);
        currentTime = p[i].ct;
    }
    
    // Draw Gantt chart footer
    printf("\n+");
    for (int i = 0; i < n + extra_cell; i++) {
        printf("-----------+");
    }
    printf("\n");
    
    // Print time markers
    currentTime = 0;
    printf("%d", currentTime);
    
    if (p[0].at > 0) {
        printf("%11d", p[0].at);
        currentTime = p[0].at;
    }
    
    for (int i = 0; i < n; i++) {
        printf("%11d", p[i].ct);
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
    int i, n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    for (i = 0; i < n; i++) {
        p[i].pid = i;
        printf("Enter Arrival Time: ");
        scanf("%d", &p[i].at);
        printf("Enter Burst Time: ");
        scanf("%d", &p[i].bt);
    }
    
    // Sort by arrival time
    ins_sort_at(n);
    
    // Calculate completion times using FCFS logic
    ct_calc_fcfs(n);
    
    // Calculate averages
    calc_avg(n);
    
    // Display the processes in execution order
    printf("\nProcess execution order: ");
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" -> ");
        printf("P%d", p[i].pid);
    }
    printf("\n");
    
    // Make a copy of the current process array for display
    struct process proc_exec_order[10];
    for (i = 0; i < n; i++) {
        proc_exec_order[i] = p[i];
    }
    
    // Sort by PID for the table display
    sort_by_pid(n);
    
    // Display results in PID order
    displaytable(n);
    
    // Use the execution order copy for the Gantt chart
    for (i = 0; i < n; i++) {
        p[i] = proc_exec_order[i];
    }
    
    // Display Gantt chart
    display(n);
    
    return 0;
}