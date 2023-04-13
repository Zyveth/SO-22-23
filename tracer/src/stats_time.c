#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "../include/message.h"
#include "../include/stats_time.h"

char* fifo_st = "/tmp/monitor";

void stats_time(char* args[], int args_num)
{
    int pid, pd[2], rets, fifo_fd;
    double exec_time = 0;
    Message m;

    rets = pipe(pd);

    if(rets == -1)
    {
        perror("Unnamed pipe error");
        exit(-1);
    }

    fifo_fd = open(fifo_st, O_WRONLY);

    if(fifo_fd == -1)
    {
        perror("Open error");
        exit(-1);
    }

    for(int i = 0; i < args_num; i++)
    {
        if((pid = fork()) == -1)
        {
            perror("Fork error");
            exit(-1);
        }

        if(pid == 0)
        {
            // Fechar extremo de leitura
            close(pd[0]);

            // Preparar FIFO de resposta (leitura)
            char fifoname[20];

            sprintf(fifoname, "/tmp/%d", getpid());

            rets = mkfifo(fifoname, 0666);

            if(rets == -1)
            {
                perror("FIFO error");
                _exit(0);
            }

            // Enviar mensagem tipo STATS_TIME ao monitor através do FIFO /tmp/monitor
            m.type = STATS_TIME;
            m.pid = getpid();
            m.pid_stats = atoi(args[i]);

            rets = write(fifo_fd, &m, sizeof(Message));

            if(rets == -1)
            {
                perror("Write error");
                _exit(0);
            }

            close(fifo_fd);

            // Ler resposta do monitor através do FIFO /tmp/(getpid())
            fifo_fd = open(fifoname, O_RDONLY);

            if(fifo_fd == -1)
            {
                perror("Open FIFO to read");
                _exit(0);
            }

            rets = read(fifo_fd, &exec_time, sizeof(double));

            if(rets == -1)
            {
                perror("Read error");
                _exit(0);
            }

            close(fifo_fd);
            unlink(fifoname);

            rets = write(pd[1], &exec_time, sizeof(double));

            if(rets == -1)
            {
                perror("Write error");
                _exit(0);
            }

            close(pd[1]);

            _exit(1);
        }
    }

    // Fechar extremo de escrita
    close(pd[1]);

    double auxiliar;
    
    for(int i = 0; i < args_num; i++)
    {
        rets = read(pd[0], &auxiliar, sizeof(double));

        if(rets == -1)
        {
            perror("Read error");
            exit(0);
        }

        exec_time += auxiliar;
    }

    printf("Total execution time is %.2f ms\n", exec_time);
}