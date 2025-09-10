/*
Implement a parent process, which sends an English alphabet to a child process 
using shared memory. The child process responds with the next English alphabet 
to the parent. The parent displays the reply from the Child.#include<unistd.h>
*/

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/wait.h>

struct alphabet_shared_st {
    int parent_written;
    int child_written;
    char alphabet;
};

char next(char c) {
    if (c == 'z') return 'a';
    else if (c == 'Z') return 'A';
    else if (isalpha(c)) return c + 1;
    else return '?';
}

void child_process(int shmid) {
    void *shared_memory = (void*)0;
    struct alphabet_shared_st *shared_stuff;
    
    shared_memory = shmat(shmid, (void*)0, 0);
    if (shared_memory == (void*)-1) {
        fprintf(stderr, "Child: shmat failed\n");
        exit(EXIT_FAILURE);
    }
    
    shared_stuff = (struct alphabet_shared_st*)shared_memory;
    
    while(1) {
        while(shared_stuff->parent_written == 0) sleep(1);
        
        printf("Child: Received '%c' from parent\n", shared_stuff->alphabet);
        
        if (shared_stuff->alphabet == '0') break;
        
        char next_char = next(shared_stuff->alphabet);
        shared_stuff->alphabet = next_char;
        
        shared_stuff->child_written = 1;
        shared_stuff->parent_written = 0;
    }
    
    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "Child: shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Child: Terminating...\n");
    exit(EXIT_SUCCESS);
}

void parent_process(int shmid) {
    void *shared_memory = (void*)0;
    struct alphabet_shared_st *shared_stuff;
    char input_char;
    int running = 1;
    
    shared_memory = shmat(shmid, (void*)0, 0);
    if (shared_memory == (void*)-1) {
        fprintf(stderr, "Parent: shmat failed\n");
        exit(EXIT_FAILURE);
    }
    
    shared_stuff = (struct alphabet_shared_st*)shared_memory;
    
    shared_stuff->parent_written = 0;
    shared_stuff->child_written = 0;
    
    printf("Parent: Enter alphabets (enter '0' to quit):\n");
    
    while(running) {
        printf("> ");
        scanf(" %c", &input_char);
        
        if (input_char == '0') {
            shared_stuff->alphabet = input_char;
            shared_stuff->parent_written = 1;
            printf("Parent: Sent '0' to child\n");
            running = 0;
            break;
        }
        
        if (!isalpha(input_char)) {
            printf("Parent: Invalid character\n");
            continue;
        }
        
        shared_stuff->alphabet = input_char;
        shared_stuff->parent_written = 1;
        
        printf("Parent: Sent '%c' to child\n", input_char);
        
        while(shared_stuff->child_written == 0) sleep(1);
        
        printf("Parent: Response: '%c'\n", shared_stuff->alphabet);
        
        shared_stuff->child_written = 0;
    }
    
    wait(NULL);

    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "Parent: shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "Parent: shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Parent: Terminated.\n");
}

int main() {
    int shmid;
    pid_t pid;
    
    shmid = shmget((key_t)1234, sizeof(struct alphabet_shared_st), 0666 | IPC_CREAT);
    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Shared memory created successfully\n");

    pid = fork();
    
    if (pid == -1) {
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) child_process(shmid);
    else parent_process(shmid);
    
    return 0;
}

/*
output:
Shared memory created successfully
Parent: Enter alphabets (enter '0' to quit):
> g
Parent: Sent 'g' to child
Child: Received 'g' from parent
Parent: Response: 'h'
> 0
Parent: Sent '0' to child
Child: Received '0' from parent
Child: Terminating...
Parent: Terminated.
*/