/*
Write two programs, producer.c implementing a producer and consumer.c
implementing a consumer, that do the following:
Your product will sit on a shelf: that is an integer - a count of the items "on the shelf".
This integer may never drop below 0 or rise above 5.
Your producer sets the value of the count to 5. It is the producer program's
responsibility to stock product on the shelf, but not overstocked. The producer may
add one item to the shelf at a time, and must report to STDOUT every time another
item is added as well as the current shelf count.
Your consumer will remove one item from the shelf at a time, provided the item count
has not dropped below zero. The consumer will decrement the counter and report the
new value to STDOUT. Have your consumer report each trip to the shelf, in which
there are no items.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FIFO_NAME "/tmp/shelf_counter"
#define MAX_SHELF 5

int main() {
    int fifo_fd;
    int current_count;
    
    if (access(FIFO_NAME, F_OK) == -1) {
        if (mkfifo(FIFO_NAME, 0666) == -1) {
            perror("mkfifo failed");
            exit(EXIT_FAILURE);
        }
    }
    
    fifo_fd = open(FIFO_NAME, O_WRONLY);
    current_count = 5;
    write(fifo_fd, &current_count, sizeof(int));
    close(fifo_fd);
    
    printf("Initialized shelf with %d items\n", current_count);
    
    while (1) {
        fifo_fd = open(FIFO_NAME, O_RDONLY);
        read(fifo_fd, &current_count, sizeof(int));
        close(fifo_fd);
        
        if (current_count < MAX_SHELF) {
            current_count++;
            printf("Added 1 item. Shelf count: %d\n", current_count);
        } else {
            printf("Shelf full\n", current_count);
        }
        
        fifo_fd = open(FIFO_NAME, O_WRONLY);
        write(fifo_fd, &current_count, sizeof(int));
        close(fifo_fd);
        
        sleep(2);
    }
    
    return 0;
}

