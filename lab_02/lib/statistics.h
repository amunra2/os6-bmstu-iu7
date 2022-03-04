#ifndef STATISTICS
#define STATISTICS

// Colors
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define TEST    "\x1b[37m"

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
