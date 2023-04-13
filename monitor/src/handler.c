#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>

#include "../include/util.h"
#include "../include/handler.h"

void create_file(Message start, Message end, char* pids_file)
{
    int fork_ret = fork();

    if(fork_ret == -1)
    {
        perror("Fork error when creating file");
        exit(-1);
    }

    if(fork_ret == 0)
    {
        char filename[20];

        sprintf(filename, "%s/%d", pids_file, start.pid);

        int fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0666);

        if(fd == -1)
        {
            perror("Open error when opening PID file");
            exit(-1);
        }

        float exec_time = calc_mili(start.timestamp, end.timestamp);

        char to_write[1024];

        sprintf(to_write, "%s\n%.2f", start.name, exec_time);

        write(fd, to_write, strlen(to_write));

        _exit(1);
    }
}

void status(HashTable h, Message m)
{
    int fork_ret = fork();

    if(fork_ret == -1)
    {
        perror("Fork error when creating file");
        exit(-1);
    }

    if(fork_ret == 0)
    {
        char fifoname[20];

        sprintf(fifoname, "/tmp/%d", m.pid);

        int fd = open(fifoname, O_WRONLY);

        if(fd == -1)
        {
            perror("Open error when opening PID FIFO");
            exit(-1);
        }

        int time_ret, write_ret;
        struct timeval now;
        char message[1024];
        float exec_time;

        for(int i = 0; i < HSIZE; i++)
        {
            for(Pair it = h[i]; it != NULL; it = it->next)
            {
                time_ret = gettimeofday(&now, NULL);

                if(time_ret == -1)
                {
                    perror("Timestamp error");
                    exit(-1);
                }

                exec_time = calc_mili(it->info.timestamp, now);

                sprintf(message, "%d %s %.2f ms\n", it->info.pid, it->info.name, exec_time);

                write_ret = write(fd, message, strlen(message));

                if(write_ret == -1)
                {
                    perror("Write error when replying status");
                    exit(-1);
                }
            }
        }

        close(fd);

        _exit(1);
    }
}

void handle_message(HashTable h, Message message, char* pids_file)
{
    if(message.type == CREATE)
        update(h, message.pid, message);
    else if(message.type == END)
    {
        Message start;
        remove_message(h, message.pid, &start);
        create_file(start, message, pids_file);
    }
    else if(message.type == STATUS)
        status(h, message);
}