/*
Write a program using semaphore to demonstrate the working of sleeping barber problem.
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_CHAIRS 3
#define NUM_CUSTOMERS 6

sem_t customers, barber, mutex;
int waiting = 0;

void *barber_thread(void *arg) {
    while (1) {
        sem_wait(&customers);
        sem_wait(&mutex);
        waiting--;
        printf("B: cut hair, wait=%d\n", waiting);
        sem_post(&mutex);
        sem_post(&barber);
        sleep(3);
    }
    return NULL;
}

void *customer_thread(void *arg) {
    int id = *((int *)arg);
    sleep(rand() % 5);
    sem_wait(&mutex);
    if (waiting < NUM_CHAIRS) {
        waiting++;
        printf("C%d: sit, wait=%d\n", id, waiting);
        sem_post(&customers);
        sem_post(&mutex);
        sem_wait(&barber);
        printf("C%d: haircut\n", id);
    } else {
        printf("C%d: leave\n", id);
        sem_post(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t bt, ct[NUM_CUSTOMERS];
    int cid[NUM_CUSTOMERS];
    srand(time(NULL));
    sem_init(&customers, 0, 0);
    sem_init(&barber, 0, 0);
    sem_init(&mutex, 0, 1);
    pthread_create(&bt, NULL, barber_thread, NULL);
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        cid[i] = i + 1;
        pthread_create(&ct[i], NULL, customer_thread, &cid[i]);
    }
    for (int i = 0; i < NUM_CUSTOMERS; i++) pthread_join(ct[i], NULL);
    pthread_cancel(bt);
    sem_destroy(&customers);
    sem_destroy(&barber);
    sem_destroy(&mutex);
    return 0;
}

/*
output:
C3: sit, wait=1
B: cut hair, wait=0
C3: haircut
C6: sit, wait=1
C2: sit, wait=2
C1: sit, wait=3
C4: leave
C5: leave
B: cut hair, wait=2
C6: haircut
B: cut hair, wait=1
C2: haircut
B: cut hair, wait=0
C1: haircut
*/