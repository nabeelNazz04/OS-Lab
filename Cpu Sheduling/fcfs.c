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
    int started; // Flag to mark if process has started
} p[10];

// Queue implementation for ready queue
struct queue {
    int items[100];
    int front;
    int rear;
    int size;
};

// Initialize queue
void initialize(struct queue *q) {
    q->front = -1;
    q->rear = -1;
    q->size = 0;
}

// Check if queue is empty
int isEmpty(struct queue *q) {
    return q->size == 0;
}

// Add element to queue
void enqueue(struct queue *q, int value) {
    if (q->size == 0) {
        q->front = 0;
        q->rear = 0;
    } else {
        q->rear = (q->rear + 1) % 100;
    }
    q->items[q->rear] = value;
    q->size++;
}

// Remove element from queue
int dequeue(struct queue *q) {
    int item = q->items[q->front];
    q->size--;
    if (q->size == 0) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front = (q->front + 1) % 100;
    }
    return item;
}

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

// Structure to store gantt chart entries
struct gantt_entry {
    int pid;  // Process ID (-1 for idle)
    int start; // Start time
    int end;   // End time
};

// Function to calculate completion times using Round Robin algorithm
void ct_calc_rr(int n, int time_quantum, struct gantt_entry chart[], int *chart_size) {
    int currentTime = 0;
    int completed = 0;
    struct queue readyQueue;
    initialize(&readyQueue);

    // Initialize remaining time and started flag
    for (int i = 0; i < n; i++) {
        p[i].rt = p[i].bt;
        p[i].started = 0;
    }
    
    int chartIndex = 0; // Index for adding Gantt chart entries
    
    while (completed < n) {
        // Check for newly arrived processes
        for (int i = 0; i < n; i++) {
            if (p[i].at <= currentTime && p[i].rt > 0 && p[i].started == 0) {
                enqueue(&readyQueue, i);
                p[i].started = 1;
            }
        }

        if (isEmpty(&readyQueue)) {
            // If the ready queue is empty, advance the current time to the next arrival time
            int nextArrivalTime = 99999;
            for (int i = 0; i < n; i++) {
                if (p[i].rt > 0 && p[i].at > currentTime && p[i].at < nextArrivalTime) {
                    nextArrivalTime = p[i].at;
                }
            }
            if (nextArrivalTime == 99999) {
                break; // No more processes to execute
            }

            //Add idle time to Gantt chart
             if (chartIndex > 0 && chart[chartIndex - 1].pid != -1) {
             chart[chartIndex].start = currentTime;
             chart[chartIndex].end = nextArrivalTime;
             chart[chartIndex].pid = -1; // Indicate idle
             chartIndex++;
             }
             currentTime = nextArrivalTime;
            continue;
        }

        int currentProcessIndex = dequeue(&readyQueue);
        int executionTime = (p[currentProcessIndex].rt < time_quantum) ? p[currentProcessIndex].rt : time_quantum;
        
        //Add process execution to Gantt chart
         chart[chartIndex].start = currentTime;
         currentTime += executionTime;
         chart[chartIndex].end = currentTime;
         chart[chartIndex].pid = p[currentProcessIndex].pid;
         chartIndex++;
        
        p[currentProcessIndex].rt -= executionTime;
        

        // If process is completed
        if (p[currentProcessIndex].rt == 0) {
            p[currentProcessIndex].ct = currentTime;
            completed++;
        } else {
            // Add back to the queue if remaining time is not zero
            for (int i = 0; i < n; i++) {
                if (p[i].at <= currentTime && p[i].rt > 0 && p[i].started == 0) {
                    enqueue(&readyQueue, i);
                    p[i].started = 1;
                }
            }

            enqueue(&readyQueue, currentProcessIndex);
        }

        // Check for newly arrived processes
        for (int i = 0; i < n; i++) {
            if (p[i].at <= currentTime && p[i].rt > 0 && p[i].started == 0) {
                enqueue(&readyQueue, i);
                p[i].started = 1;
            }
        }
    }
    *chart_size = chartIndex;
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
    printf("Average waiting time: %.6f\n", avg_wt);
    printf("Average turn around time: %.6f\n", avg_tat);
}

// Function to display Gantt chart as shown in the sample
void display_gantt(struct gantt_entry chart[], int chart_size) {
    printf("\nGantt Chart:\n");
    
    // Print top border
    printf("+");
    for (int i = 0; i < chart_size; i++) {
        for (int j = 0; j < 4; j++) { // Adjusted for shorter blocks
            printf("---+");
        }
    }
    printf("\n|");

    // Print process IDs or "idle"
    for (int i = 0; i < chart_size; i++) {
        if (chart[i].pid == -1) {
            printf(" idle |");
        } else {
            printf(" P%d  |", chart[i].pid + 1);
        }
    }
    printf("\n+");
      for (int i = 0; i < chart_size; i++) {
        for (int j = 0; j < 4; j++) { // Adjusted for shorter blocks
            printf("---+");
        }
    }
    printf("\n");

    // Print time markers
    printf("0");
    for (int i = 0; i < chart_size; i++) {
        printf("%4d", chart[i].end);
    }
    printf("\n");
}

// Function to display process details in a table as shown in the sample
void displaytable(int n) {
    printf("\nProcess Table:\n");
    printf("-------------------------------------------------------\n");
    printf("| PID | AT | BT | CT | TAT | WT |\n");
    printf("-------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("| %2d | %2d | %2d | %2d | %3d | %2d |\n",
               p[i].pid + 1, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    printf("-------------------------------------------------------\n");
}

// Function to sort by PID for final display
void sort_by_pid(int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (p[j].pid > p[j + 1].pid) {
                struct process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

int main() {
    int n, time_quantum, i;
    
    // Get input from the user
    printf("Enter the number of process: ");
    scanf("%d", &n);
    printf("Enter the quantum time: ");
    scanf("%d", &time_quantum);
    
    for (i = 0; i < n; i++) {
        p[i].pid = i;
        printf("Enter id, arrival time, burst time of process %d (separated by spaces): ", i + 1);
        scanf("%d %d %d", &p[i].pid, &p[i].at, &p[i].bt);
        p[i].pid--; // Adjust pid to be 0-indexed internally
        p[i].started = 0;
        p[i].rt = p[i].bt;
    }
    
    // Sort by arrival time first
    ins_sort_at(n);
    
    // Create array for gantt chart entries
    struct gantt_entry chart[100];
    int chart_size = 0;
    
    // Calculate completion times using Round Robin logic
    ct_calc_rr(n, time_quantum, chart, &chart_size);
    
    // Calculate averages
    calc_avg(n);
    
    // Sort by PID for table output
    sort_by_pid(n);
    
    // Display results with processes sorted by PID
    displaytable(n);
    
    // Display Gantt chart
    display_gantt(chart, chart_size);
    
    return 0;
}
