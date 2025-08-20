#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

int main(int argc, char **argv) {
    int fd = open("wordlist.txt", O_RDONLY);
    char buffer[1024];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer)-1);
    buffer[bytes_read] = 0;
    char *line = strtok(buffer, "\n");
    while(line != NULL) {
        if(strstr(line, argv[1])) printf("%s\n", line);
        line = strtok(NULL, "\n");
    }
    return 0;
}
