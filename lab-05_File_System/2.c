/*
Write a C program to load the binary executable of the previous program in a child
process using the exec system call.
*/

#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main() {
    pid_t pid;
    pid = fork();
    if(pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(-1);
    }
    else if(pid == 0) execlp("./1.out", "./1.out", NULL);
    else {
        wait(NULL);
        printf("previous program executed by child\n");
        exit(0);
    }
}

/*
output:
I'm the child!
I'm the parent!
Child returned 0
previous program executed by child
*/