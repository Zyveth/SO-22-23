#ifndef MESSAGE_H
#define MESSAGE_H

#include <sys/time.h>
#include <sys/stat.h>

#define CREATE 0
#define END 1
#define STATUS 2
#define STATS_TIME 3

typedef struct message
{
    int type;
    int pid;
    char name[20];
    struct timeval timestamp;
    int pid_stats;
} Message;

#endif