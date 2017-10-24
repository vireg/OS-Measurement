#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <time.h>

#define CHUNK 65536
//#define SIZE 1073741824
//4294967296
#define CYCLE (1/(2.6*1000000000))
#define BILLION 1000000000

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

int main(int argc, const char * argv[])
{
    if (argc < 2) {
	printf("usage: ./fsc.o FILE_SIZE\n");
	return 0;
    }
    unsigned int SIZE = atoi(argv[1]);
    char data[CHUNK];
    size_t val=0, total_bytes=0;
    char filename[] = "fsc";
    int fd = open(filename, O_CREAT | O_RDWR | O_SYNC);	//O_CREAT | O_RDWR, S_IRUSR | S_IWUSR
    struct timespec ckstart, ckstop;
    double accum=0, tt=0;

    if (fd < 0) {
        printf("Open failed. Exiting...\n");
        return -1;
    }

    //unsigned long size = 4294967296; //536870912; //1073741824; //1GB
    ftruncate(fd, SIZE);

    lseek(fd, 0, SEEK_SET);

    while(1){
	if((val = read(fd, data, CHUNK)) <= 0) {
		printf("Done...\n");
		break;	//read CHUNK, advances pointer by CHUNK
	}

	//printf("val = %zu, total_bytes = %zu\n",val, total_bytes);
	if (total_bytes>=SIZE) {
		printf("Read %zu bytes\n",total_bytes);break;
	}
	total_bytes += val;
        //lseek(fd, -2*CHUNK, SEEK_CUR);			//seek back by 2 CHUNKs
    }
    
    close(fd);

    fd = open(filename, O_RDONLY| O_SYNC);
    lseek(fd, -CHUNK, SEEK_END);
    
    uint64_t start=0,end=0,total_time=0,diff=0;
    total_bytes=0;
    while(1){

        //start = rdtsc();
	clock_gettime( CLOCK_REALTIME, &ckstart);
        val = read(fd, data, CHUNK);
	clock_gettime( CLOCK_REALTIME, &ckstop);
	accum = ( ckstop.tv_sec - ckstart.tv_sec )*1000000 + ( ckstop.tv_nsec - ckstart.tv_nsec ) / 1000;
	//accum = (ckstop.tv_nsec - ckstart.tv_nsec)/1000;
        //end = rdtscp();
        if (val<=0) {
		printf("Done.\n");
		break;	//read CHUNK, advances pointer by CHUNK
	}
	total_time += (end - start);
	tt += accum;
	diff=end-start;
//if(SIZE==2147483648) printf("%zu bytes\n",total_bytes);
	//if (diff>9999) {	//break when spike in time to read
	//	printf("diff:%ld\tbytes: %zu\n",diff,total_bytes);
		//break;
	//}
	if (total_bytes>=SIZE) {
		printf("Read %zu bytes\n",total_bytes);break;
	}
	total_bytes += val;
        lseek(fd, -2*CHUNK, SEEK_CUR);			//seek back by 2 CHUNKs
    }
    
    close(fd);
    double norm_time = (double)CYCLE*total_time*1000000/(SIZE/CHUNK);
    printf("%lf num_chunks=%d\n******************************Done.\n", (double)tt/(SIZE/CHUNK),SIZE/CHUNK);
    FILE* file = fopen("fsc_results.txt","a+");
    fprintf(file,"%lf\n",tt);
    fclose(file);
    return 0;
}


