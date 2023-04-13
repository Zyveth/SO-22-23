#include <string.h>
#include <sys/time.h>

#include "../include/util.h"

float calc_mili(struct timeval begin, struct timeval end)
{
    float seg = (float) end.tv_sec - (float) begin.tv_sec;
    seg *= 1000;

    float micro = (float) end.tv_usec - (float) begin.tv_usec;
    micro /= 1000;

    return seg + micro;
}