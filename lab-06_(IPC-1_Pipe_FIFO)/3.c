/*
Write a C program to implement one side of FIFO.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_NAME "/tmp/test_fifo"
#define BUFFER_SIZE 256

int main() {
    int fd;
    char buffer[BUFFER_SIZE];
    
    if (mkfifo(FIFO_NAME, 0666) == -1) perror("mkfifo");
    
    printf("FIFO Writer: Opening FIFO for writing...\n");
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    
    printf("Enter messages (type 'quit' to exit):\n");
    
    while (1) {
        printf("Message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        if (strncmp(buffer, "quit", 4) == 0) break;
        write(fd, buffer, strlen(buffer));
        printf("Sent: %s", buffer);
    }
    
    close(fd);
    unlink(FIFO_NAME);
    printf("FIFO Writer: Finished\n");
    return 0;
}
