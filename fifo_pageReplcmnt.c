#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_PAGES 100
#define MAX_FRAMES 10

// Function to implement FIFO page replacement algorithm
void fifo(int p[], int numP, int numF) {
    int frames[MAX_FRAMES];
    int front = 0;
    int hit = 0, fault = 0;

    // Initialize frames with -1
    for (int i = 0; i < numF; i++) {
        frames[i] = -1;
    }

    printf("\nFIFO Page Replacement:\n");
    for (int i = 0; i < numP; i++) {
        int pg = p[i];
        int found = 0;

        // Check if page is already in frames
        for (int j = 0; j < numF; j++) {
            if (frames[j] == pg) {
                found = 1;
                hit++;
                break;
            }
        }

        if (!found) {
            // If frames are full, replace the oldest page
            frames[front] = pg;
            front = (front + 1) % numF; // Circular queue
            fault++;
        }

        printf("Step %d\t:", i + 1);
        for (int j = 0; j < numF; j++) {
            if (frames[j] != -1) {
                printf("%d\t", frames[j]);
            } else {
                printf("-\t");
            }
        }

        printf("%s\n", found ? "HIT" : "FAULT");
    }

    printf("Total Page Hits: %d\n", hit);
    printf("Total Page Faults: %d\n", fault);
    printf("Page Hit Ratio: %f\n", ((float)hit / (hit + fault)));
    printf("Page Fault Ratio: %f\n", ((float)fault / (hit + fault)));
}

// Function to implement LRU page replacement algorithm
void lru(int p[], int numP, int numF) {
    int frames[MAX_FRAMES];
    int last[MAX_FRAMES];
    int hit = 0, fault = 0;

    // Initialize frames and last access time
    for (int i = 0; i < numF; i++) {
        frames[i] = -1;
        last[i] = -1;
    }

    printf("\nLRU Page Replacement:\n");
    for (int i = 0; i < numP; i++) {
        int pg = p[i];
        int found = 0;

        // Check if page is already in frames
        for (int j = 0; j < numF; j++) {
            if (frames[j] == pg) {
                found = j;
                hit++;
                last[found] = i;
                break;
            }
        }

        if (found == 0) {
            // If page not found, replace the least recently used page
            int lru_index = 0;
            for (int j = 1; j < numF; j++) {
                if (last[j] < last[lru_index]) {
                    lru_index = j;
                }
            }

            if (frames[lru_index] == -1) {
                // If frame is empty, just add the page
                frames[lru_index] = pg;
            } else {
                // Replace the LRU page
                frames[lru_index] = pg;
            }

            last[lru_index] = i;
            fault++;
        }

        printf("Step %d\t:", i + 1);
        for (int j = 0; j < numF; j++) {
            if (frames[j] != -1) {
                printf("%d\t", frames[j]);
            } else {
                printf("-\t");
            }
        }

        printf("%s\n", found != 0 ? "HIT" : "FAULT");
    }

    printf("Total Page Hits: %d\n", hit);
    printf("Total Page Faults: %d\n", fault);
    printf("Page Hit Ratio: %f\n", ((float)hit / (hit + fault)));
    printf("Page Fault Ratio: %f\n", ((float)fault / (hit + fault)));
}

// Function to implement LFU page replacement algorithm
void lfu(int p[], int numP, int numF) {
    int frames[MAX_FRAMES];
    int last[MAX_FRAMES];
    int freq[MAX_FRAMES];
    int hit = 0, fault = 0;

    // Initialize frames, last access time, and frequency
    for (int i = 0; i < numF; i++) {
        frames[i] = -1;
        last[i] = -1;
        freq[i] = 0;
    }

    printf("\nLFU Page Replacement:\n");
    for (int i = 0; i < numP; i++) {
        int pg = p[i];
        int found = -1;

        // Check if page is already in frames
        for (int j = 0; j < numF; j++) {
            if (frames[j] == pg) {
                found = j;
                hit++;
                freq[found]++;
                last[found] = i;
                break;
            }
        }

        if (found == -1) {
            // If page not found, replace the least frequently used page
            int lfu_index = 0;
            for (int j = 1; j < numF; j++) {
                if (freq[j] < freq[lfu_index] || (freq[j] == freq[lfu_index] && last[j] < last[lfu_index])) {
                    lfu_index = j;
                }
            }

            if (frames[lfu_index] == -1) {
                // If frame is empty, just add the page
                frames[lfu_index] = pg;
            } else {
                // Replace the LFU page
                frames[lfu_index] = pg;
            }

            freq[lfu_index] = 1;
            last[lfu_index] = i;
            fault++;
        }

        printf("Step %d\t:", i + 1);
        for (int j = 0; j < numF; j++) {
            if (frames[j] != -1) {
                printf("%d\t", frames[j]);
            } else {
                printf("-\t");
            }
        }

        printf("%s\n", found != -1 ? "HIT" : "FAULT");
    }

    printf("Total Page Hits: %d\n", hit);
    printf("Total Page Faults: %d\n", fault);
    printf("Page Hit Ratio: %f\n", ((float)hit / (hit + fault)));
    printf("Page Fault Ratio: %f\n", ((float)fault / (hit + fault)));
}

int main() {
    int numP, numF, choice;
    int pages[MAX_PAGES];

    printf("Enter the number of pages: \n");
    scanf("%d", &numP);

    printf("Enter the page references:\n");
    for (int i = 0; i < numP; i++) {
        scanf("%d", &pages[i]);
    }

    printf("Enter the number of frames:\n");
    scanf("%d", &numF);

    while (1) {
        printf("Choose a page replacement algorithm:\n");
        printf("1. FIFO\n");
        printf("2. LRU\n");
        printf("3. LFU\n");
        printf("4. Exit\n");

        scanf("%d", &choice);

        switch (choice) {
            case 1:
                fifo(pages, numP, numF);
                break;
            case 2:
                lru(pages, numP, numF);
                break;
            case 3:
                lfu(pages, numP, numF);
                break;
            case 4:
                exit(0);
            default:
                printf("Enter a valid choice!\n");
                break;
        }
    }

    return 0;
}