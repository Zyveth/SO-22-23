#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>

#include "../include/util.h"
#include "../include/message.h"

int handle_create(int pid, char* name, struct timeval timestamp)
{
    char filename[20];

    sprintf(filename, "./pids/%d", pid);

    int fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0666);

    if(fd == -1)
    {
        perror("Open error -> create");
        exit(-1);
    }

    char to_write[1024];

    sprintf(to_write, "%s\n%ld %d\n", name, timestamp.tv_sec, timestamp.tv_usec);

    write(fd, to_write, strlen(to_write));

    return 0;
}

void handle_message(Message message)
{
    int fork_ret = fork();

    if(fork_ret == -1)
    {
        perror("Fork error -> handle_message");
        exit(-1);
    }

    if(fork_ret == 0)
    {
        switch (message.type)
        {
            case CREATE:
                handle_create(message.pid, message.name, message.timestamp);
                break;
        
            default:
                break;
        }
    }
}