/*
Demonstrate creation, writing to, and reading from a pipe.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char write_msg[] = "Hello from parent process!";
    char read_msg[100];
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }
    
    printf("Pipe created successfully\n");
    
    pid = fork();
    
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    
    if (pid == 0) {
        close(pipefd[1]);
        
        printf("Child: Reading from pipe...\n");
        int bytes_read = read(pipefd[0], read_msg, sizeof(read_msg));
        if (bytes_read > 0) {
            read_msg[bytes_read] = '\0';
            printf("Child: Received message: %s\n", read_msg);
        }
        
        close(pipefd[0]);
        exit(0);
    } else {
        close(pipefd[0]);
        
        printf("Parent: Writing to pipe...\n");
        write(pipefd[1], write_msg, strlen(write_msg));
        printf("Parent: Sent message: %s\n", write_msg);
        
        close(pipefd[1]);
        wait(NULL);
        printf("Parent: Child process finished\n");
    }
    
    return 0;
}

/*
output:
Pipe created successfully
Parent: Writing to pipe...
Parent: Sent message: Hello from parent process!
Child: Reading from pipe...
Child: Received message: Hello from parent process!
Parent: Child process finished
*/