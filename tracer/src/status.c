#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "../include/message.h"
#include "../include/readln.h"
#include "../include/status.h"

char* fifo_s = "/tmp/monitor";

void status()
{
    int pid = getpid();

    char fifoname[20];

    sprintf(fifoname, "/tmp/%d", pid);

    int rets = mkfifo(fifoname, 0666);

    if(rets == -1)
    {
        perror("Mkfifo error");
        exit(-1);
    }

    int fd = open(fifo_s, O_WRONLY);

    if(fd == -1)
    {
        perror("Open error");
        exit(-1);
    }

    Message m;
    m.type = STATUS;
    m.pid = pid;

    rets = write(fd, &m, sizeof(Message));

    if(rets == -1)
    {
        perror("Writing status");
        exit(-1);
    }

    close(fd);

    fd = open(fifoname, O_RDONLY);

    char buffer[1024];

    while((rets = readln(fd, buffer, 1024)) > 0)
        printf("%s", buffer);

    close(fd);

    unlink(fifoname);
}