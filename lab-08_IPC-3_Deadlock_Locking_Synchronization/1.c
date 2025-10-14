/*
Modify the above Producer-Consumer program so that, a producer can produce at the most
10 items more than what the consumer has consumed.
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 100
#define MAX_ITEMS 100
#define MAX_AHEAD 10

int buffer[BUFFER_SIZE];
int produce_ptr = 0, consume_ptr = 0;
sem_t mutex, full, empty, ahead;

void *producer(void *arg) {
    for (int item = 0; item < MAX_ITEMS; item++) {
        sem_wait(&ahead);
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[produce_ptr] = item;
        printf("P[%d]=%d\n", produce_ptr, item);
        produce_ptr = (produce_ptr + 1) % BUFFER_SIZE;
        sem_post(&mutex);
        sem_post(&full);
        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        sem_wait(&full);
        sem_wait(&mutex);
        int item = buffer[consume_ptr];
        printf("C[%d]=%d\n", consume_ptr, item);
        consume_ptr = (consume_ptr + 1) % BUFFER_SIZE;
        sem_post(&mutex);
        sem_post(&empty);
        sem_post(&ahead);
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&ahead, 0, MAX_AHEAD);
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
    sem_destroy(&ahead);
    return 0;
}

/*
P[0]=0
C[0]=0
P[1]=1
C[1]=1
P[2]=2
P[3]=3
C[2]=2
.
.
.
P[18]=18
P[19]=19
C[10]=10
P[20]=20
C[11]=11
P[21]=21
C[12]=12
P[22]=22
*/