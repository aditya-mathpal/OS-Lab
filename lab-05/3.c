/*
Write a program to create a child process. Display the process IDs of the process, parent and child (if any) in both the parent and child processes.
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main() {
    int status;
    pid_t pid;
    pid = fork();

    if(pid == -1)
        printf("\nERROR, child not created");
    else if (pid == 0) {
        printf("\nI'm the child!\nMy PID is %d\nMy parent's PID is %d\n", getpid(), getppid());
        exit(0);
    }
    else {
        wait(&status);
        printf("\nI'm the parent!\nMy PID is %d\nMy child's PID is %d\n", getpid(), pid);
    }
}

/*
output:
I'm the child!
My PID is 87924
My parent's PID is 87923

I'm the parent!
My PID is 87923
My child's PID is 87924
*/