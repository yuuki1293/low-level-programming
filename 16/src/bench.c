#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include "bmp.h"

void bench(void (*target)(struct image*), const char* image_name ,const char* const label, int count)
{
    int i;
    char* filename = malloc(sizeof(char) * 30);
    char* image_name_o = malloc(sizeof(char) * 30);
    FILE* fp;
    struct bmp_header header;
    struct image image;
    struct rusage r;
    struct timeval start, end;
    long res;

    sprintf(filename, "%s.csv", label);
    fp = fopen(filename, "w");
    sprintf(image_name_o, "%s.bmp", label);

    for (i = 0; i < count; i++)
    {
        load_bmp(image_name, &header, &image);

        getrusage(RUSAGE_SELF, &r);
        start = r.ru_utime;

        (*target)(&image);

        getrusage(RUSAGE_SELF, &r);
        end = r.ru_utime;
        res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
        
        save_bmp(image_name_o, &header, &image);

        fprintf(fp, "%ld\n", res);
#ifdef LOG
        printf("(%d/%d)%s: %ldμs\n", i+1, count, label, res);
#endif
    }

    fclose(fp);
}