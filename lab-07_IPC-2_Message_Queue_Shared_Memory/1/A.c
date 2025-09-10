/*
Process A wants to send a number to Process B. Once received, Process B has to check 
whether  the  number  is  palindrome  or  not.  Write  a  C  program  to  implement  this 
interprocess communication using a message queue. 
*/

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<sys/ipc.h>

struct msg_st {
    long int msg_type;
    int number;
};

int main() {
    int running = 1;
    struct msg_st data;
    int msgid;
    int input;

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    printf("Message queue created...\n");

    while(running) {
        printf("Enter a number (0 to exit): ");
        scanf("%d", &input);
        
        if (input == 0) running = 0;

        data.msg_type = 1;
        data.number = input;

        if (msgsnd(msgid, (void*)&data, sizeof(int), 0) == -1) {
            fprintf(stderr, "msgsnd failed with error: %d\n", errno);
            exit(EXIT_FAILURE);
        }

        printf("Sent %d to B\n", input);
    }

    exit(EXIT_SUCCESS);
}
