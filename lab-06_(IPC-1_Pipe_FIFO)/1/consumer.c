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

#define FIFO_NAME "/tmp/my_fifo"

int main() {
    int fd;
    int number;
    
    printf("Opening FIFO for reading...\n");
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    
    printf("Reading integers from FIFO:\n");
    for (int i = 0; i < 4; i++) {
        if (read(fd, &number, sizeof(int)) > 0) {
            printf("Read %d\n", number);
        }
    }
    
    close(fd);
    unlink(FIFO_NAME); // Remove FIFO
    printf("Finished\n");
    return 0;
}
