/*
Write two programs named Interface and Display for the following problem. 
Interface program 
This program, when run, gives a prompt to the user as “Enter your message:”When the 
user enters his/her message string and presses Enter, the program writes the message into 
the shared memory, tells the Display to start processing,and then prompts the user again 
for another message. 
Display program 
The process waits until a new message becomes available on the shared memory. Then it 
reads the contents of the memory and prints it on the screen. It also clears the contents of 
the shared memory when it has read the message.
*/

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include "message_com.h"

int main() {
    int running = 1;
    void *shared_memory = (void*)0;
    struct message_shared_st *shared_stuff;
    int shmid;

    shmid = shmget((key_t)1234, sizeof(struct message_shared_st), 0666 | IPC_CREAT);
    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    shared_memory = shmat(shmid, (void*)0, 0);
    if (shared_memory == (void*)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Waiting...\n");
    
    shared_stuff = (struct message_shared_st*)shared_memory;

    while(running) {
        while(shared_stuff->message_ready == 0) sleep(1);

        printf("Received message: %s", shared_stuff->message_text);

        if(strncmp(shared_stuff->message_text, "exit", 4) == 0) running = 0;

        memset(shared_stuff->message_text, 0, TEXT_SZ);

        shared_stuff->message_ready = 0;
        shared_stuff->message_processed = 1;

    }

    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Terminated.\n");
    exit(EXIT_SUCCESS);
}
