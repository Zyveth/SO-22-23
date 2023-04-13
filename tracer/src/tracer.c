#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/system.h"
#include "../include/util.h"
#include "../include/status.h"

int main(int argc, char const *argv[])
{
    if(argc < 2)
    {
        perror("Invalid argument number!");
        exit(-1);
    }

    if(strcmp(argv[1], "execute") == 0)
    {
        if(argc != 4)
        {
            perror("Invalid argument number!");
            exit(-1);
        }

        if(strcmp(argv[2], "-u") == 0)
            _system_((char*) argv[3]);
    }
    else if(strcmp(argv[1], "status") == 0)
        status();
        
    return 0;
}