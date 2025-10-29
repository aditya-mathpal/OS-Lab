/*
Simulate implementation of Disk Scheduling Algorithms: FCFS, SSTF using a 
structure DSA. An DSA contains the request ID, arrival timestamp, cylinder, address, 
and the ID of the process that posted the request. 
struct DSA { 
int request_id; 
Int arrival_time_stamp; 
Int cylinder; 
Int address; 
int process_id; 
}#include <stdio.h>
*/

#include<stdio.h>
#include <stdlib.h>

struct DSA {
    int request_id;
    int arrival_time_stamp;
    int cylinder;
    int address;
    int process_id;
};

void fcfs(struct DSA req[], int n, int head) {
    int i, total=0, curr=head;
    printf("\nFCFS Sequence: %d", head);
    for(i=0; i<n; i++) {
        printf(" -> %d", req[i].cylinder);
        total += abs(curr - req[i].cylinder);
        curr = req[i].cylinder;
    }
    printf("\nTotal head movement: %d\n", total);
}

void sstf(struct DSA req[], int n, int head) {
    int i, j, total=0, curr=head, vis[n];
    printf("\nSSTF Sequence: %d", head);
    for(i=0; i<n; i++) vis[i]=0;
    for(i=0; i<n; i++) {
        int min=9999, idx=-1;
        for(j=0; j<n; j++) {
            if(!vis[j]) {
                int d = abs(curr - req[j].cylinder);
                if(d < min) {
                    min = d;
                    idx = j;
                }
            }
        }
        vis[idx] = 1;
        printf(" -> %d", req[idx].cylinder);
        total += min;
        curr = req[idx].cylinder;
    }
    printf("\nTotal head movement: %d\n", total);
}

int main() {
    int n, head, i;
    printf("Enter number of requests: ");
    scanf("%d", &n);
    struct DSA req[n];
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("\nEnter request details:\n");
    for(i=0; i<n; i++) {
        req[i].request_id = i+1;
        printf("Request %d - Arrival time: ", i+1);
        scanf("%d", &req[i].arrival_time_stamp);
        printf("Cylinder: ");
        scanf("%d", &req[i].cylinder);
        printf("Address: ");
        scanf("%d", &req[i].address);
        printf("Process ID: ");
        scanf("%d", &req[i].process_id);
    }
    fcfs(req, n, head);
    sstf(req, n, head);
    return 0;
}

/*
output:
Enter number of requests: 3
Enter initial head position: 50

Enter request details:
Request 1 - Arrival time: 1
Cylinder: 98
Address: 1000
Process ID: 1
Request 2 - Arrival time: 2
Cylinder: 183
Address: 2000 
Process ID: 2
Request 3 - Arrival time: 3
Cylinder: 37
Address: 3000
Process ID: 3

FCFS Sequence: 50 -> 98 -> 183 -> 37
Total head movement: 279

SSTF Sequence: 50 -> 37 -> 98 -> 183
Total head movement: 159
*/