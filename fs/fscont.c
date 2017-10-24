#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <time.h>

#define SIZE (65536*256)
#define CHUNK 65536
#define NUM_PROCS 9
#define CYCLE (1/(2.6*1000000000))
#define ITER 1000


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

void file_access(char *file){
  char* data = malloc(CHUNK*sizeof(char));
  
  int fd = open(file, O_RDONLY | O_SYNC);
  int i,j,k;
  struct timespec ckstart, ckstop;
  double accum=0, tt=0;

  int num_chunks = SIZE/CHUNK;

  uint64_t start=0,end=0,total_time=0;

  for (i = 0; i < ITER; i++) {
    j = rand() % num_chunks;
    lseek(fd, j*CHUNK, SEEK_SET);
    start = rdtsc();
    //clock_gettime( CLOCK_REALTIME, &ckstart);
    read(fd, data, CHUNK);
    //clock_gettime( CLOCK_REALTIME, &ckstop);
    //accum = ( ckstop.tv_sec - ckstart.tv_sec )*1000000 + ( ckstop.tv_nsec - ckstart.tv_nsec ) / 1000;
    end = rdtscp();
    //tt += accum;
    //printf("%lf\n",accum);
    total_time += (end - start);
  }

  close(fd);
  double norm_time = (double)CYCLE*total_time*1000000/ITER;
  printf("%lf\n", norm_time);

}

int main(int argc, char const *argv[])
{
	pid_t procid[NUM_PROCS];
	char* filename[] = {"filex0", "filex1", "filex2", "filex3", "filex4", "filex5", "filex6", "filex7", "filex8", "filex9"};
	int i, fd;
	
	for (i=0;i<10;i++) {
		fd = open(filename[i], O_CREAT | O_RDWR | O_SYNC);
		if (fd < 0) {
			printf("Open failed. Exiting...\n");
			return -1;
	    	}
		ftruncate(fd, SIZE);
		close(fd);
	}

	for (i = 0; i < NUM_PROCS; ++i) {
	  if ((procid[i] = fork()) != 0) {
	    //printf("Process %d created...\n",i);
	    file_access(filename[i]);
	    exit(0);
	  } else {
	  }
	}

	return 0;

}


