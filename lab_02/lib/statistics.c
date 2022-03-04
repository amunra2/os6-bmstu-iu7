#include "statistics.h"


float findPercent(int part, int all)
{
    return part * 100 / all;
}



void printStat(stat_t *stats)
{
    stats->ntotal = stats->nreg + \
                    stats->nchr + \
                    stats->nblk + \
                    stats->nfifo + \
                    stats->nslink + \
                    stats->nsock + \
                    stats->ndir;

    printf(YELLOW
        "\n\n\n Статистика файлов \
         \n ------------------------------------------------------------------ \
         \n Обычные:                                   %5d (%4.2f %%)          \
         \n Каталоги:                                  %5d (%4.2f %%)          \
         \n Специальные файлы блочных устройств:       %5d (%4.2f %%)          \
         \n Специальные файлы символьных устройств:    %5d (%4.2f %%)          \
         \n FIFO:                                      %5d (%4.2f %%)          \
         \n Символьные ссылки:                         %5d (%4.2f %%)          \
         \n Сокеты:                                    %5d (%4.2f %%)          \
         \n ------------------------------------------------------------------ \
         \n Всего:                                     %5d\n\n" RESET, 

         stats->nreg, findPercent(stats->nreg, stats->ntotal),
         stats->ndir, findPercent(stats->ndir, stats->ntotal),
         stats->nblk, findPercent(stats->nblk, stats->ntotal),
         stats->nchr, findPercent(stats->nchr, stats->ntotal),
         stats->nfifo, findPercent(stats->nfifo, stats->ntotal),
         stats->nslink, findPercent(stats->nslink, stats->ntotal),
         stats->nsock, findPercent(stats->nsock, stats->ntotal),
         stats->ntotal
    );
}


void incStat(struct stat *mode, stat_t *stats)
{
    switch (mode->st_mode & __S_IFMT)
    {
    case __S_IFREG:
        stats->nreg++;
        break;
    case __S_IFBLK:
        stats->nblk++;
        break;
    case __S_IFCHR:
        stats->nchr++;
        break;
    case __S_IFIFO:
        stats->nfifo++;
        break;
    case __S_IFLNK:
        stats->nslink++;
        break;
    case __S_IFSOCK:
        stats->nsock++;
        break;
    }
}
