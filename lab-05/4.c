/*
Create a zombie (defunct) child process (a child with exit() call, but no corresponding wait() in the sleeping parent) and allow the init process to adopt it (after parent terminates). Run the process as a background process and run the “ps” command.
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main() {
    pid_t pid, pid2;
    pid = fork();

    if(pid == -1)
        printf("\nERROR, child not created");
    else if (pid == 0) {
        printf("\nI'm the child!\nNow I'm a zombie\nRun ps -ef | grep 4.out to find me\n");
        exit(0);
    }
    else {
        sleep(10);
        printf("\nParent dying, init will clean up zombie\nRun ps -ef | grep 4.out to check\n");
        exit(0);
    }
    return 0;
}

/*
output:
I'm the child!
Now I'm a zombie
Run ps -ef | grep 4.out to find me

Parent dying, init will clean up zombie
Run ps -ef | grep 4.out to check
*/