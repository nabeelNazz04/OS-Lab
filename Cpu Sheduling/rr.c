#include <stdio.h>
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

// Function to sort processes by arrival time
void sort_by_at(int n) {
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

    // Sort processes by arrival time
    sort_by_at(n);

    // Enqueue processes that arrive at time 0
    for (int i = 0; i < n; i++) {
        if (p[i].at == 0) {
            enqueue(&readyQueue, i);
            p[i].started = 1;
        }
    }

    *chart_size = 0;
    int prev_pid = -1;
    int prev_time = 0;

    // Continue until all processes are completed
    while (completed < n) {
        int idx = -1;

        // If ready queue is empty
        if (isEmpty(&readyQueue)) {
            // Find next process to arrive
            int next_at = 999999;
            int next_proc = -1;

            for (int i = 0; i < n; i++) {
                if (p[i].rt > 0 && p[i].at < next_at && p[i].at > currentTime) {
                    next_at = p[i].at;
                    next_proc = i;
                }
            }

            // If there's idle time
            if (next_proc != -1) {
                // Add idle entry to gantt chart
                if (prev_pid != -1) {
                    chart[*chart_size].pid = prev_pid;
                    chart[*chart_size].start = prev_time;
                    chart[*chart_size].end = currentTime;
                    (*chart_size)++;
                }
                chart[*chart_size].pid = -1; // -1 represents idle
                chart[*chart_size].start = currentTime;
                chart[*chart_size].end = next_at;
                (*chart_size)++;

                prev_pid = -1;
                prev_time = next_at;
                currentTime = next_at;

                // Add process to ready queue
                enqueue(&readyQueue, next_proc);
                p[next_proc].started = 1;
            } else {
                break; // No more processes to execute
            }
        } else {
            // Get process from ready queue
            idx = dequeue(&readyQueue);

            // Calculate execution time for this cycle
            int exec_time = (p[idx].rt < time_quantum) ? p[idx].rt : time_quantum;

            // Update current time
            currentTime += exec_time;
            p[idx].rt -= exec_time;

            // Add to gantt chart if different from previous process
            if (prev_pid != idx) {
                if (prev_pid != -1) {
                    chart[*chart_size].pid = prev_pid;
                    chart[*chart_size].start = prev_time;
                    chart[*chart_size].end = currentTime - exec_time;
                    (*chart_size)++;
                }
                prev_pid = idx;
                prev_time = currentTime - exec_time;
            }

            // Check for new arrivals during this time slice
            for (int i = 0; i < n; i++) {
                if (p[i].rt > 0 && p[i].at <= currentTime && p[i].started == 0) {
                    enqueue(&readyQueue, i);
                    p[i].started = 1;
                }
            }

            // If process still has remaining time, put it back in ready queue
            if (p[idx].rt > 0) {
                enqueue(&readyQueue, idx);
            } else {
                // Process completed
                p[idx].ct = currentTime;
                completed++;
            }
        }
    }

    // Add final process to gantt chart
    if (prev_pid != -1) {
        chart[*chart_size].pid = prev_pid;
        chart[*chart_size].start = prev_time;
        chart[*chart_size].end = currentTime;
        (*chart_size)++;
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
    printf("Average waiting time: %.6f\n", avg_wt);
    printf("Average turn around time: %.6f\n", avg_tat);
}

// Function to display Gantt chart as shown in the sample
void display_gantt(struct gantt_entry chart[], int chart_size) {
    printf("\nGantt Chart:\n");

    // Print top border
    printf("+");
    for (int i = 0; i < chart_size; i++) {
        for (int j = 0; j < 10; j++) {
            printf("+");
        }
    }
    printf("+\n|");

    // Print process IDs
    for (int i = 0; i < chart_size; i++) {
        if (chart[i].pid == -1) {
            printf("    idle   |");
        } else {
            printf("    P%d     |", chart[i].pid + 1);
        }
    }

    // Print second row (with double vertical lines)
    printf("\n|");
    for (int i = 0; i < chart_size; i++) {
        printf("    ||     |");
    }

    // Print time markers
    printf("\n%d", chart[0].start);
    for (int i = 0; i < chart_size; i++) {
        printf("          %d", chart[i].end);
    }
    printf("\n");
}

// Function to display process details in a table as shown in the sample
void displaytable(int n) {
    printf("\nProcess Table:\n");
    printf("+-----+------------+------------+------------+------------+------------+\n");
    printf("| PID | Arrival    | Burst      | Completion | Turnaround | Waiting    |\n");
    printf("|     | Time       | Time       | Time       | Time       | Time       |\n");
    printf("+-----+------------+------------+------------+------------+------------+\n");
    for (int i = 0; i < n; i++) {
        printf("| %3d | %8d   | %8d   | %8d   | %8d   | %8d   |\n",
               p[i].pid + 1, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    printf("+-----+------------+------------+------------+------------+------------+\n");
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
    int n, time_quantum;

    // Get input from the user
    printf("Enter the number of process: ");
    scanf("%d", &n);
    printf("Enter the quantum time: ");
    scanf("%d", &time_quantum);

    for (int i = 0; i < n; i++) {
        p[i].pid = i;
        printf("Enter id, arrival time, burst time of process %d (separated by spaces): ", i + 1);
        scanf("%d %d %d", &p[i].pid, &p[i].at, &p[i].bt);
        p[i].pid--; // Adjust pid to be 0-indexed internally
    }

    // Sort by arrival time first
    sort_by_at(n);

    // Create array for gantt chart entries
    struct gantt_entry chart[100];
    int chart_size = 0;

    // Calculate completion times using Round Robin logic
    ct_calc_rr(n, time_quantum, chart, &chart_size);

    // Calculate averages
    calc_avg(n);

    // Sort by PID for table output
    sort_by_pid(n);

    // Display results with processes sorted by process ID
    displaytable(n);

    // Display Gantt chart
    display_gantt(chart, chart_size);

    return 0;
}
