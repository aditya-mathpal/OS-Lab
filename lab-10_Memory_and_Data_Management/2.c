/*
Write a C program using Malloc for implementing Multilevel feedback queue using three  
queues  with  each  of  them  working  with  different  scheduling  policies
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct Process {
    int pid, aT, bT, rT, qL, cT, taT, wT;
    struct Process *next;
} Process;

typedef struct Queue {
    Process *front;
    Process *rear;
} Queue;

Queue* createQueue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, Process *p) {
    if (q->rear == NULL) {
        q->front = q->rear = p;
        p->next = NULL;
        return;
    }
    q->rear->next = p;
    q->rear = p;
    p->next = NULL;
}

Process* dequeue(Queue *q) {
    if (q->front == NULL) return NULL;
    Process *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    temp->next = NULL;
    return temp;
}

int isEmpty(Queue *q) {
    return q->front == NULL;
}

Process* findSJF(Queue *q) {
    if (isEmpty(q)) return NULL;
    
    Process *shortest = q->front;
    Process *current = q->front;
    
    while (current != NULL) {
        if (current->rT < shortest->rT) {
            shortest = current;
        }
        current = current->next;
    }
    
    if (shortest == q->front) {
        return dequeue(q);
    } else {
        Process *prev = q->front;
        while (prev->next != shortest) {
            prev = prev->next;
        }
        prev->next = shortest->next;
        if (shortest == q->rear) {
            q->rear = prev;
        }
        shortest->next = NULL;
        return shortest;
    }
}

void mlfqScheduler(Process *processes[], int n) {
    Queue *q1 = createQueue();
    Queue *q2 = createQueue();
    Queue *q3 = createQueue();
    
    int currentTime = 0;
    int completed = 0;
    int idx = 0;

    printf("Queue 1: Round Robin (Time Quantum = 4)\n");
    printf("Queue 2: Round Robin (Time Quantum = 8)\n");
    printf("Queue 3: SJF (Non-preemptive)\n\n");
    
    for (int i = 0; i < n; i++) {
        if (processes[i]->aT == 0) {
            enqueue(q1, processes[i]);
            idx++;
        }
    }
    
    printf("Execution Timeline:\n");
    printf("%-8s %-10s %-8s %-30s\n", "Time", "Process", "Queue", "Action");
    printf("----------------------------------------------------------------\n");
    
    while (completed < n) {
        while (idx < n && processes[idx]->aT <= currentTime) {
            enqueue(q1, processes[idx]);
            printf("%-8d %-10s %-8s %-30s\n", 
                   currentTime, "", "", "P" + processes[idx]->pid);
            idx++;
        }
        
        if (!isEmpty(q1)) {
            Process *p = dequeue(q1);
            int timeSlice = (p->rT < 4) ? p->rT : 4;
            
            printf("%-8d P%-9d Q1       Executing\n", 
                   currentTime, p->pid);
            
            currentTime += timeSlice;
            p->rT -= timeSlice;
            
            while (idx < n && processes[idx]->aT <= currentTime) {
                enqueue(q1, processes[idx]);
                idx++;
            }
            
            if (p->rT == 0) {
                p->cT = currentTime;
                p->taT = p->cT - p->aT;
                p->wT = p->taT - p->bT;
                completed++;
                printf("%-8d P%-9d Q1       Completed\n", currentTime, p->pid);
            } else {
                p->qL = 2;
                enqueue(q2, p);
                printf("%-8d P%-9d Q1→Q2    Demoted\n", currentTime, p->pid);
            }
        }
        else if (!isEmpty(q2)) {
            Process *p = dequeue(q2);
            int timeSlice = (p->rT < 8) ? p->rT : 8;
            
            printf("%-8d P%-9d Q2       Executing\n", 
                   currentTime, p->pid);
            
            currentTime += timeSlice;
            p->rT -= timeSlice;
            
            while (idx < n && processes[idx]->aT <= currentTime) {
                enqueue(q1, processes[idx]);
                idx++;
            }
            
            if (p->rT == 0) {
                p->cT = currentTime;
                p->taT = p->cT - p->aT;
                p->wT = p->taT - p->bT;
                completed++;
                printf("%-8d P%-9d Q2       Completed\n", currentTime, p->pid);
            } else {
                p->qL = 3;
                enqueue(q3, p);
                printf("%-8d P%-9d Q2→Q3    Demoted\n", currentTime, p->pid);
            }
        }
        else if (!isEmpty(q3)) {
            Process *p = findSJF(q3);
            
            printf("%-8d P%-9d Q3       Executing\n", 
                   currentTime, p->pid);
            
            currentTime += p->rT;
            p->rT = 0;
            p->cT = currentTime;
            p->taT = p->cT - p->aT;
            p->wT = p->taT - p->bT;
            completed++;
            
            while (idx < n && processes[idx]->aT <= currentTime) {
                enqueue(q1, processes[idx]);
                idx++;
            }
            
            printf("%-8d P%-9d Q3       Completed\n", currentTime, p->pid);
        } else {
            currentTime++;
        }
    }
    
    printf("\nProcess Statistics:\n");
    printf("%-8s %-12s %-12s %-12s %-12s %-12s\n", 
           "PID", "Arrival", "Burst", "Completion", "Turnaround", "Waiting");
    printf("------------------------------------------------------------------------\n");
    
    float totalTAT = 0, totalWT = 0;
    for (int i = 0; i < n; i++) {
        printf("P%-7d %-12d %-12d %-12d %-12d %-12d\n", 
               processes[i]->pid, 
               processes[i]->aT,
               processes[i]->bT,
               processes[i]->cT,
               processes[i]->taT,
               processes[i]->wT);
        totalTAT += processes[i]->taT;
        totalWT += processes[i]->wT;
    }
    
    printf("\nAvg TAT: %.2f\n", totalTAT / n);
    printf("Average WT: %.2f\n", totalWT / n);
}

int main() {
    int n;
    
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    Process **processes = (Process**)malloc(n * sizeof(Process*));
    
    printf("\nEnter process details:\n");
    for (int i = 0; i < n; i++) {
        processes[i] = (Process*)malloc(sizeof(Process));
        processes[i]->pid = i + 1;
        
        printf("%d:\n", i + 1);
        printf("  AT: ");
        scanf("%d", &processes[i]->aT);
        printf("  BT: ");
        scanf("%d", &processes[i]->bT);
        
        processes[i]->rT = processes[i]->bT;
        processes[i]->qL = 1;
        processes[i]->next = NULL;
    }
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j]->aT > processes[j + 1]->aT) {
                Process *temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
    
    mlfqScheduler(processes, n);
    
    for (int i = 0; i < n; i++)
        free(processes[i]);
    free(processes);
    
    return 0;
}

/*
output:
Enter number of processes: 3

Enter process details:
1:
  AT: 0
  BT: 16
2:
  AT: 2
  BT: 10
3:
  AT: 5
  BT: 10
Queue 1: Round Robin (Time Quantum = 4)
Queue 2: Round Robin (Time Quantum = 8)
Queue 3: SJF (Non-preemptive)

Execution Timeline:
Time     Process    Queue    Action                        
----------------------------------------------------------------
0        P1         Q1       Executing
4        P1         Q1→Q2    Demoted
4        P2         Q1       Executing
8        P2         Q1→Q2    Demoted
8        P3         Q1       Executing
12       P3         Q1→Q2    Demoted
12       P1         Q2       Executing
20       P1         Q2→Q3    Demoted
20       P2         Q2       Executing
26       P2         Q2       Completed
26       P3         Q2       Executing
32       P3         Q2       Completed
32       P1         Q3       Executing
36       P1         Q3       Completed

Process Statistics:
PID      Arrival      Burst        Completion   Turnaround   Waiting     
------------------------------------------------------------------------
P1       0            16           36           36           20          
P2       2            10           26           24           14          
P3       5            10           32           27           17          

Avg TAT: 29.00
Average WT: 17.00
*/