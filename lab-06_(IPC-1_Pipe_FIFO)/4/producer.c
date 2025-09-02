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

int main() {
    int fd;
    char buf[16];
    int count;
    const char *file = "shelf.txt";

    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    sprintf(buf, "5\n");
    write(fd, buf, strlen(buf));
    close(fd);
    printf("Set shelf count to 5\n");

    while (1) {
        sleep(2);
        fd = open(file, O_RDWR);
        if (fd == -1) {
            perror("open");
            exit(1);
        }
        int n = read(fd, buf, sizeof(buf) - 1);
        buf[n] = '\0';
        count = atoi(buf);
        if (count < 5) {
            count++;
            sprintf(buf, "%d\n", count);
            lseek(fd, 0, SEEK_SET);
            write(fd, buf, strlen(buf));
            printf("Added one item, current shelf count %d\n", count);
        }
        close(fd);
    }
    return 0;
}

