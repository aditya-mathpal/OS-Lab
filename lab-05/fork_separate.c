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
    else if(pid == 0) execlp("/bin/ls", "ls", NULL);
    else {
        wait(NULL);
        printf("child complete\n");
        exit(0);
    }
}