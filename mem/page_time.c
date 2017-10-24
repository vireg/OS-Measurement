#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define CYCLE 1/(2.6*1000000000)
#define READ_OH 32	// Obtained from previous measurement
#define LOOP_OH 6	// Obtained from previous measurement
#define ITERATIONS 15
#define LOOP_TIMES 100

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


int main() {
    int fd = open("filex", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    FILE *file1;
    file1 = fopen("page_times.txt","a+");

    if (fd < 0) {
        printf("Open failed. Exiting...\n");
        return -1;
    }

    unsigned long offset = 1073741824; //8388608; //8MB
    unsigned long size = 4294967296; //536870912; //1073741824; //1GB
    ftruncate(fd, size);
    char* map = (char*) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map < 0) {
        printf("Map failed. Exiting...\n");
        return -1;
    }

    //printf("Done.\n");

    char c; int i,j,k;
    uint64_t start, end, diff;
    double time;

	    start = rdtsc();
	    for (j = 0; j < LOOP_TIMES; j++)
	    {   
		//printf("j = %d\n",j);
		c = map[(j*offset + rand()%offset)%size];
	    }
	    end = rdtscp();
	    diff = end - start;
	    
	
	    time = (double)(diff - LOOP_OH - READ_OH)*CYCLE/LOOP_TIMES;
	    printf("time: %lf\n", time);
	    fprintf(file1, "%lf\n", time);


    munmap(map, size);
    close(fd);
    fclose(file1);

    return 0;
}

