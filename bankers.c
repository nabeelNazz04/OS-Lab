#include <stdio.h>

void main() {
    int n, m, i, j, k, y, alloc[20][20], max[20][20], avail[50], ind = 0;
    
    printf("Enter the number of Processes: ");
    scanf("%d", &n);
    printf("Enter the number of Resources: ");
    scanf("%d", &m);
    
    printf("Enter the Allocation Matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            scanf("%d", &alloc[i][j]);
        }
    }
    
    printf("Enter the Max Matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            scanf("%d", &max[i][j]);
        }
    }
    
    printf("Enter the Available Matrix:\n");
    for (i = 0; i < m; i++) {
        scanf("%d", &avail[i]);
    }
    
    int finish[n], safesequence[n], work[m], need[n][m];
    
    // Calculate NEED matrix
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
    
    printf("\nNEED Matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
    
    // Initialize work array
    for (i = 0; i < m; i++) {
        work[i] = avail[i];
    }
    
    // Initialize finish array to 0
    for (i = 0; i < n; i++) {
        finish[i] = 0;
    }
    
    // Banker's Algorithm
    int count = 0;
    while (count < n) {
        int found = 0;
        for (i = 0; i < n; i++) {
            if (finish[i] == 0) {
                int flag = 1;
                // Check if need[i] <= work
                for (j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        flag = 0;
                        break;
                    }
                }
                if (flag) {
                    safesequence[ind++] = i;
                    for (y = 0; y < m; y++) {
                        work[y] += alloc[i][y];
                    }
                    finish[i] = 1;
                    found = 1;
                    count++;
                }
            }
        }
        if (!found) {
            printf("\nSystem is in unsafe state!\n");
            return 1;
        }
    }
    
    printf("\nSafe Sequence: ");
    for (i = 0; i < n; i++) {
        printf("P%d ", safesequence[i]);
    }
    printf("\n");
    
}
