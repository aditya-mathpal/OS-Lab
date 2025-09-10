#define TEXT_SZ 2048

struct message_shared_st {
    int message_ready;
    int message_processed;
    char message_text[TEXT_SZ];
};