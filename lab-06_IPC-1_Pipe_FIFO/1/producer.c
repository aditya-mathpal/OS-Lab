/*
Write a producer and consumer program in C using the FIFO queue. The producer
should write a set of 4 integers into the FIFO queue and the consumer should display
the 4 integers.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_NAME "/tmp/my_fifo"

int main() {
    int fd;
    int numbers[4] = {10, 20, 30, 40};
    
    if (access(FIFO_NAME, F_OK) == -1) {
        if (mkfifo(FIFO_NAME, 0666) == -1) {
            perror("mkfifo");
            exit(1);
        }
    }
    
    printf("Opening FIFO for writing...\n");
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    
    printf("Writing 4 integers to FIFO\n");
    for (int i = 0; i < 4; i++) {
        write(fd, &numbers[i], sizeof(int));
        printf("Wrote %d\n", numbers[i]);
    }
    
    close(fd);
    printf("Finished\n");
    return 0;
}
