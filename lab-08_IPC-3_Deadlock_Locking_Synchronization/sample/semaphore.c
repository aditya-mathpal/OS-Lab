#include<pthread.h>
#include<stdio.h>
#include<semaphore.h>

sem_t semaphore;
void *func1(void *param) {
    printf("Thread 1\n");
    sem_post(&semaphore);
}
void *func2(void* param) {
    sem_wait(&semaphore);
    printf("Thread 2\n");
}