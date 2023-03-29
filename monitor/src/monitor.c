#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "../include/handler.h"
#include "../include/message.h"

int fd;
char* fifo = "/tmp/monitor";

void signal_callback_handler(int signum)
{
    printf("Closing file descriptor.\n");
    int close_ret = close(fd);

    if(close_ret == -1)
    {
        perror("Close error");
        exit(-1);
    }

    printf("Closing named pipe.\n");
    close_ret = unlink(fifo);

    if(close_ret != 0)
    {
        perror("Unlink error");
        exit(-1);
    }

    exit(signum);
}

int main(int argc, char const *argv[])
{
    int bytes_read;
    Message message;

    printf("Making named pipe.\n");
    int mkfifo_ret = mkfifo(fifo, 0666);

    if(mkfifo_ret == -1)
    {
        perror("Mkfifo error");
        exit(-1);
    }

    signal(SIGINT, signal_callback_handler);

    printf("Opening reading end.\n");
    fd = open(fifo, O_RDONLY);

    if(fd == -1)
    {
        perror("Open error");
        exit(-1);
    }

    while(1)
    {
        bytes_read = read(fd, &message, sizeof(message));

        if(bytes_read == -1)
        {
            perror("Read error -> monitor");
            exit(-1);
        }

        printf("%d %d %s %ld %d\n", message.type, message.pid, message.name, message.timestamp.tv_sec, message.timestamp.tv_usec);
        // handle_message(message);
    }

    return 0;
}