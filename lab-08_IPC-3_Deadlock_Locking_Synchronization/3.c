/*
Write a Code to access a shared resource which causes deadlock using improper use of
semaphore.
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t r1, r2;

void *thread1(void *arg) {
    printf("T1: wait r1\n");
    sem_wait(&r1);
    printf("T1: got r1\n");
    sleep(2);
    printf("T1: wait r2\n");
    sem_wait(&r2);
    printf("T1: got r2\n");
    sem_post(&r2);
    sem_post(&r1);
    return NULL;
}

void *thread2(void *arg) {
    printf("T2: wait r2\n");
    sem_wait(&r2);
    printf("T2: got r2\n");
    sleep(2);
    printf("T2: wait r1\n");
    sem_wait(&r1);
    printf("T2: got r1\n");
    sem_post(&r1);
    sem_post(&r2);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    sem_init(&r1, 0, 1);
    sem_init(&r2, 0, 1);
    pthread_create(&t1, NULL, thread1, NULL);
    sleep(1);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    sem_destroy(&r1);
    sem_destroy(&r2);
    return 0;
}

/*
output:
T1: wait r1
T1: got r1
T2: wait r2
T2: got r2
T1: wait r2
T2: wait r1
*/