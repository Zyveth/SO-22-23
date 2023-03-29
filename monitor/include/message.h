#ifndef MESSAGE_H
#define MESSAGE_H

#include <sys/time.h>
#include <sys/stat.h>

#define CREATE 0

typedef struct message
{
    int type;
    int pid;
    char name[20];
    struct timeval timestamp;
} Message;

#endif