/*
Write a program to list the files given as arguments, stopping every 20 lines until a
key is hit. (a simple version of more UNIX utility)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define LINES_PER_PAGE 20
int main(int argc, char *argv[]) {
    if (argc < 2) {
        write(2, "Usage: ./3.out file1 file2 ...\n", 31);
        return 1;
    }
    char buffer[1024];
    int line_count = 0;
    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            perror("open");
            continue;
        }
        ssize_t bytes_read;
        while ((bytes_read = read(fd, buffer, sizeof(buffer)-1)) > 0) {
            buffer[bytes_read] = 0;
            char *ptr = buffer;
            while (*ptr) {
                if (*ptr == '\n') {
                    line_count++;
                    if (line_count >= LINES_PER_PAGE) {
                        char c;
                        read(0, &c, 1);
                        line_count = 0;
                        ptr++;
                    }
                }
                write(1, ptr, 1);
                ptr++;
            }
        }
        if (bytes_read < 0) {
            perror("read");
        }
        close(fd);
    }
    return 0;
}

/*
output:

$ 3.out sample_text.txt
one
two
three
four
five
six
seven
eight
nine
ten
eleven
twelve
thirteen
fourteen
fifteen
sixteen
seventeen
eighteen
nineteen
twenty
*/
