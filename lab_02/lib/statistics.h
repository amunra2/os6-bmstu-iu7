#ifndef STATISTICS
#define STATISTICS

// Colors
#define RESET "\033[0m"

#include <sys/stat.h>
#include <stdio.h>


typedef struct stat_s
{
    long nreg;
    long ndir;
    long nblk;
    long nchr;
    long nfifo;
    long nslink;
    long nsock;
    long ntotal;
} stat_t;


void printStat(stat_t *stats);
void incStat(struct stat *mode, stat_t *stats);

#endif
