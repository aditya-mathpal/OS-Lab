/*
Consider the following snapshot of the system. Write C program to implement Banker’s 
algorithm for deadlock avoidance. The program has to accept all inputs from the user. 
Assume the total number of instances of A,B and C are 10,5 and 7 respectively.
*/

#include <stdio.h>

int main() {
    int n=5, m=3, i, j, k, count=0, alloc[5][3] = {{0,1,0}, {2,0,0}, {3,0,2}, {2,1,1}, {0,0,2}}, max[5][3] = {{7,5,3}, {3,2,2}, {9,0,2}, {2,2,2}, {4,3,3}}, avail[3] = {3,3,2}, need[5][3], finish[5] = {0}, safe[5];

    // printf("Allocation Matrix:\n");
    // for(i=0; i<n; i++) {
    //     printf("P%d: ", i);
    //     for(j=0; j<m; j++)
    //         printf("%d ", alloc[i][j]);
    //     printf("\n");
    // }

    // printf("\nMax Matrix:\n");
    // for(i=0; i<n; i++) {
    //     printf("P%d: ", i);
    //     for(j=0; j<m; j++)
    //         printf("%d ", max[i][j]);
    //     printf("\n");
    // }

    // printf("\nAvailable Matrix:\n");
    // for(j=0; j<m; j++)
    //     printf("%d ", avail[j]);
    // printf("\n");
    
    for(i=0; i<n; i++)
        for(j=0; j<m; j++)
            need[i][j] = max[i][j] - alloc[i][j];
    
    for(k=0; k<n; k++) {
        for(i=0; i<n; i++) {
            if(!finish[i]) {
                int flag=1;
                for(j=0; j<m; j++)
                    if(need[i][j] > avail[j]) {
                        flag=0;
                        break;
                    }
                if(flag) {
                    safe[count++] = i;
                    finish[i] = 1;
                    for(j=0; j<m; j++)
                        avail[j] += alloc[i][j];
                }
            }
        }
    }
    
    printf("\nNeed Matrix:\n");
    for(i=0; i<n; i++) {
        printf("P%d: ", i);
        for(j=0; j<m; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }
    
    printf("\nSafe Sequence: ");
    for(i=0; i<n; i++)
        printf("P%d ", safe[i]);
    printf("\n");
    
    return 0;
}

/*
ans:
(a)
Need Matrix:
P0: 7 4 3 
P1: 1 2 2 
P2: 6 0 0 
P3: 0 1 1 
P4: 4 3 1

(b)
Safe Sequence: P1 P3 P4 P0 P2
all 5 processes, therefore system is safe

(c)
Yes, because request is <= Need[P1] and Available
Updated matrices:

Allocation:
P0: 0 1 0 
P1: 3 0 2 
P2: 3 0 2 
P3: 2 1 1 
P4: 0 0 2

Need:
P0: 7 4 3 
P1: 0 2 0 
P2: 6 0 0 
P3: 0 1 1 
P4: 4 3 1

Available:
2 3 0

(d)
Yes, request <= Need[P4] and Available

(e)
Yes, request <= Need[P0] and Available
*/