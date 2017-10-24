#define _GNU_SOURCE

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/stat.h>

#define CHUNK 4096
#define CYCLE 1/(2.6*1000000000)

static inline uint64_t rdtsc(void) {
    uint32_t cycles_low, cycles_high;
    asm volatile ("cpuid\n\t" "rdtsc\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" : "=r" (cycles_high), "=r" (cycles_low) :: "%rax", "%rbx", "%rcx", "%rdx");
    return (((uint64_t)cycles_high << 32) | cycles_low);
}

static inline uint64_t rdtscp(void) {
    uint32_t cycles_low, cycles_high;
    asm volatile ("rdtscp\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" "cpuid\n\t" : "=r" (cycles_high), "=r" (cycles_low) :: "%rax", "%rbx", "%rcx", "%rdx");
    return (((uint64_t)cycles_high << 32) | cycles_low);
}

int main(int argc, char** argv)
{
    //if (argc < 2) {
//	printf("usage: ./seq.o FILE_SIZE\n");
//	return 0;
    //}
    unsigned long SIZE = 4096*1000;//atoi(argv[1]);
    char data[CHUNK];
    size_t val=0, total_bytes=0;
    char filename[] = "/mnt/vrege/new_folder/app-debug.apk";
    int fd = open(filename, O_RDWR | O_SYNC);	//O_CREAT | O_RDWR, S_IRUSR | S_IWUSR
    struct timespec ckstart, ckstop;
    double accum=0, tt=0;

    if (fd < 0) {
        printf("Open failed. Exiting...\n");
        return -1;
    }

    //unsigned long size = 4294967296; //536870912; //1073741824; //1GB
    //ftruncate(fd, SIZE);

    uint64_t start=0,end=0,total_time=0;
    lseek(fd, 0, SEEK_SET);

    while (1) {
//        start = rdtsc();
	clock_gettime( CLOCK_REALTIME, &ckstart);
        val = read(fd, data, CHUNK);
	clock_gettime( CLOCK_REALTIME, &ckstop);
//        end = rdtscp();
	accum = ( ckstop.tv_sec - ckstart.tv_sec )*1000000 + ( ckstop.tv_nsec - ckstart.tv_nsec ) / 1000;
	tt += accum;
	total_bytes += val;
        if (val <= 0) {
	    printf("Done.\n");
            break;
        }
	if (total_bytes >= SIZE) {
	    printf("Done...\n");
            break;
	}
        total_time += (end - start);
    }
    close(fd);
    double num_chunks = SIZE/CHUNK;
    double norm_time = CYCLE*total_time/num_chunks;	//time to read 4kB chunk

    printf("read time = %lf\n", tt/num_chunks);
    //FILE* file = fopen("fsq_results.txt","a+");
    //fprintf(file,"%lf\n",tt);
    //fclose(file);

    return 0;

}

