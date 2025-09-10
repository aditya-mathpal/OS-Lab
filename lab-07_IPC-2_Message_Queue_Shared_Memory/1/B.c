/*
Process A wants to send a number to Process B. Once received, Process B has to check 
whether  the  number  is  palindrome  or  not.  Write  a  C  program  to  implement  this 
interprocess communication using a message queue. #include<unistd.h>
*/

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

int is_palindrome(int num) {
    int og = num;
    int rev = 0;
    int rem;
    if (num < 0) return 0;
    while (num != 0) {
        rem = num % 10;
        rev = rev * 10 + rem;
        num /= 10;
    }
    return (og == rev);
}

int main() {
    int running = 1;
    int msgid;
    struct msg_st data;
    long int msg_to_receive = 0;

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    printf("Waiting...\n");

    while(running) {
        if (msgrcv(msgid, (void*)&data, sizeof(int), msg_to_receive, 0) == -1) {
            fprintf(stderr, "msgrcv failed with error: %d\n", errno);
            exit(EXIT_FAILURE);
        }

        printf("Received %d\n", data.number);

        if (data.number == 0) {
            printf("Received exit signal\n");
            running = 0;
        } else {
            if (is_palindrome(data.number)) printf("palindrome\n", data.number);
            else printf("not palindrome\n", data.number);
        }
    }

    if (msgctl(msgid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Exiting...\n");
    exit(EXIT_SUCCESS);
}
