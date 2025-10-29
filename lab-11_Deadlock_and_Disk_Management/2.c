/*
Write a multithreaded program that implements the banker's algorithm. Create n threads 
that request and release resources from the bank. The banker will grant the request only 
if it leaves the system in a safe state. You may write this program using pthreads. It is 
important  that  shared  data  be  safe  from  concurrent  access.  To  ensure  safe  access  to 
shared data, you can use mutex locks, which are available in the pthreads libraries.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int n=5, m=3;
int avail[3] = {3,3,2};
int max[5][3] = {{7,5,3}, {3,2,2}, {9,0,2}, {2,2,2}, {4,3,3}};
int alloc[5][3] = {{0,1,0}, {2,0,0}, {3,0,2}, {2,1,1}, {0,0,2}};
int need[5][3];
pthread_mutex_t lock;

int is_safe() {
    int work[3], fin[5]={0}, i, j, k, cnt=0;
    for(i=0; i<m; i++) work[i] = avail[i];
    for(k=0; k<n; k++) {
        for(i=0; i<n; i++) {
            if(!fin[i]) {
                int ok=1;
                for(j=0; j<m; j++)
                    if(need[i][j] > work[j]) {
                        ok=0;
                        break;
                    }
                if(ok) {
                    fin[i]=1;
                    cnt++;
                    for(j=0; j<m; j++) work[j] += alloc[i][j];
                }
            }
        }
    }
    return cnt==n;
}

int request(int pid, int req[]) {
    pthread_mutex_lock(&lock);
    int i, ok=1;
    for(i=0; i<m; i++)
        if(req[i]>need[pid][i]) {ok=0; break;}
    if(ok) {
        for(i=0; i<m; i++)
            if(req[i]>avail[i]) {ok=0; break;}
    }
    if(ok) {
        for(i=0; i<m; i++) {
            avail[i] -= req[i];
            alloc[pid][i] += req[i];
            need[pid][i] -= req[i];
        }
        if(!is_safe()) {
            for(i=0; i<m; i++) {
                avail[i] += req[i];
                alloc[pid][i] -= req[i];
                need[pid][i] += req[i];
            }
            ok=0;
        }
    }
    pthread_mutex_unlock(&lock);
    return ok;
}

void release(int pid, int rel[]) {
    pthread_mutex_lock(&lock);
    for(int i=0; i<m; i++) {
        avail[i] += rel[i];
        alloc[pid][i] -= rel[i];
        need[pid][i] += rel[i];
    }
    pthread_mutex_unlock(&lock);
}

void* process(void* arg) {
    int pid = *(int*)arg;
    int req[3], i;
    for(int t=0; t<3; t++) {
        for(i=0; i<m; i++) req[i] = rand() % (need[pid][i]+1);
        printf("P%d requesting: (%d,%d,%d)\n", pid, req[0], req[1], req[2]);
        if(request(pid, req))
            printf("P%d: GRANTED\n", pid);
        else
            printf("P%d: DENIED\n", pid);
        sleep(1);
        for(i=0; i<m; i++) req[i] = rand() % (alloc[pid][i]+1);
        printf("P%d releasing: (%d,%d,%d)\n", pid, req[0], req[1], req[2]);
        release(pid, req);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t th[5];
    int ids[5];
    int i, j;
    pthread_mutex_init(&lock, NULL);
    for(i=0; i<n; i++)
        for(j=0; j<m; j++)
            need[i][j] = max[i][j] - alloc[i][j];
    for(i=0; i<n; i++) {
        ids[i] = i;
        pthread_create(&th[i], NULL, process, &ids[i]);
    }
    for(i=0; i<n; i++) pthread_join(th[i], NULL);
    pthread_mutex_destroy(&lock);
    printf("\nFinal Available: (%d,%d,%d)\n", avail[0], avail[1], avail[2]);
    return 0;
}

/*
output:
P0 requesting: (7,1,1)
P0: DENIED
P2 requesting: (5,0,0)
P2: DENIED
P3 requesting: (0,0,1)
P3: GRANTED
P1 requesting: (0,0,0)
P1: GRANTED
P4 requesting: (0,3,1)
P4: DENIED
P0 releasing: (0,0,0)
P3 releasing: (1,0,2)
P2 releasing: (0,0,0)
P1 releasing: (2,0,0)
P4 releasing: (0,0,1)
P0 requesting: (1,2,2)
P0: GRANTED
P4 requesting: (3,1,0)
P4: DENIED
P3 requesting: (1,0,1)
P3: GRANTED
P2 requesting: (2,0,0)
P2: GRANTED
P1 requesting: (1,2,2)
P1: DENIED
P0 releasing: (1,2,2)
P3 releasing: (2,0,1)
P4 releasing: (0,0,0)
P2 releasing: (2,0,2)
P1 releasing: (0,0,0)
P0 requesting: (1,0,0)
P0: GRANTED
P3 requesting: (0,0,2)
P3: GRANTED
P4 requesting: (1,1,1)
P2 requesting: (0,0,0)
P2: GRANTED
P4: GRANTED
P1 requesting: (2,0,2)
P1: GRANTED
P0 releasing: (1,0,0)
P3 releasing: (0,0,1)
P2 releasing: (0,0,0)
P4 releasing: (0,0,0)
P1 releasing: (2,0,1)

Final Available: (6,2,3)
*/