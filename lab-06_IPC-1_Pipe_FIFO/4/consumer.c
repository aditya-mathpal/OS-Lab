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
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/shelf_counter"

int main() {
    int fifo_fd;
    int current_count;
    
    printf("Consumer starting...\n");
    
    while (1) {
        fifo_fd = open(FIFO_NAME, O_RDONLY);
        if (read(fifo_fd, &current_count, sizeof(int)) > 0) {
            close(fifo_fd);
            
            if (current_count > 0) {
                current_count--;
                printf("Removed 1 item. New shelf count: %d\n", current_count);
            } else printf("No items available\n");
            
            fifo_fd = open(FIFO_NAME, O_WRONLY);
            write(fifo_fd, &current_count, sizeof(int));
            close(fifo_fd);
        } else {
            close(fifo_fd);
        }
        
        sleep(3);
    }
    
    return 0;
}

