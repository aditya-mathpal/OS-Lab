/*
Write a C program for the first readers-writers problem using semaphores.
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 1
#define NUM_WRITERS 2
#define READ_COUNT 2
#define WRITE_COUNT 2

sem_t mutex, wrt;
int readcount = 0, shared_data = 0;

void *reader(void *arg) {
    int id = *((int *)arg);
    for (int i = 0; i < READ_COUNT; i++) {
        sem_wait(&mutex);
        readcount++;
        if (readcount == 1) sem_wait(&wrt);
        sem_post(&mutex);
        printf("R%d: data=%d cnt=%d\n", id, shared_data, readcount);
        sleep(1);
        sem_wait(&mutex);
        readcount--;
        if (readcount == 0) sem_post(&wrt);
        sem_post(&mutex);
        sleep(1);
    }
    return NULL;
}

void *writer(void *arg) {
    int id = *((int *)arg);
    for (int i = 0; i < WRITE_COUNT; i++) {
        sem_wait(&wrt);
        shared_data++;
        printf("W%d: data=%d\n", id, shared_data);
        sleep(2);
        sem_post(&wrt);
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t r[NUM_READERS], w[NUM_WRITERS];
    int rid[NUM_READERS], wid[NUM_WRITERS];
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);
    for (int i = 0; i < NUM_READERS; i++) {
        rid[i] = i + 1;
        pthread_create(&r[i], NULL, reader, &rid[i]);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        wid[i] = i + 1;
        pthread_create(&w[i], NULL, writer, &wid[i]);
    }
    for (int i = 0; i < NUM_READERS; i++) pthread_join(r[i], NULL);
    for (int i = 0; i < NUM_WRITERS; i++) pthread_join(w[i], NULL);
    sem_destroy(&mutex);
    sem_destroy(&wrt);
    printf("Final=%d\n", shared_data);
    return 0;
}

/*
output:
W1: data=1
R1: data=1 cnt=1
W2: data=2
R1: data=2 cnt=1
W1: data=3
W2: data=4
Final=4
*/