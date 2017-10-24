#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define LOOP_TIMES 10
#define MEGABYTE 1048576
#define CYCLE 1/(2.6*1000000000)
#define READ_OH 32	// Obtained from previous measurement
#define LOOP_OH 6	// Obtained from previous measurement

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

double writeTime(int *array, int size);
double readTime(int *array, int size);

int main(int argc, char const *argv[])
{
	int size = 33554432;//67108864;	//64*4 = 256MB
	int i,j,k;
	int *array = (int *)calloc(size, sizeof(int));

	double wr_bw;
	double rd_bw;
	int count = 10;

	FILE *file1, *file2;
	file1 = fopen("RAM_wr_bw.txt","w+");
	file2 = fopen("RAM_rd_bw.txt","w+");
	for (i=0;i<count;i++){
		wr_bw = writeTime(array, size);
		rd_bw = readTime(array, size);
		printf("wr_bw = %f\n",wr_bw);
		printf("rd_bw = %f\n",rd_bw);
		fprintf(file1, "%f\n", wr_bw);
		fprintf(file2, "%f\n", rd_bw);
	}

	fclose(file1);
	fclose(file2);

	return 0;
}


double writeTime(int* array, int size) {
	unsigned long long t0, t1, diff;
	int i,j,k;

	t0 = rdtsc();
	for (j=0;j<LOOP_TIMES;j++) {
		for (k=0;k<size;k += 1) {
			array[k] = k;		// rand()%size
/*
			array[k+1] = 0;
			array[k+2] = 0;
			array[k+3] = 0;			
			array[k+4] = 0;
			array[k+5] = 0;
			array[k+6] = 0;
			array[k+7] = 0;
			array[k+8] = 0;
			array[k+9] = 0;
			array[k+10] = 0;			
			array[k+11] = 0;
			array[k+12] = 0;
			array[k+13] = 0;
			array[k+14] = 0;					
			array[k+15] = 0;
			array[k+16] = 0;
			array[k+17] = 0;
			array[k+18] = 0;			
			array[k+19] = 0;
			array[k+20] = 0;
			array[k+21] = 0;
			array[k+22] = 0;
			array[k+23] = 0;
			array[k+24] = 0;
			array[k+25] = 0;			
			array[k+26] = 0;
			array[k+27] = 0;
			array[k+28] = 0;
			array[k+29] = 0;					
			array[k+30] = 0;										
			array[k+31] = 0;
*/							
		}
	}
	t1 = rdtscp();
	diff = t1 - t0;

	double bw = 4*(double)size*LOOP_TIMES/(MEGABYTE*(double)(diff - 2*LOOP_OH - READ_OH)*CYCLE);
	return bw;
}


double readTime(int *array, int size) {
	unsigned long long t0, t1, diff;
	int i,j,k;
	unsigned long long a = 0;

	t0 = rdtsc();
	for (j=0;j<LOOP_TIMES;j++) {
		for (k=0;k<size;k += 1) {
			a += array[k];	// rand()%size
/*
			a += array[k+1];
			a += array[k+2];
			a += array[k+3];
			a += array[k+4];
			a += array[k+5];
			a += array[k+6];
			a += array[k+7];
			a += array[k+8];
			a += array[k+9];
			a += array[k+10];
			a += array[k+11];
			a += array[k+12];
			a += array[k+13];
			a += array[k+14];
			a += array[k+15];
			a += array[k+16];
			a += array[k+17];
			a += array[k+18];
			a += array[k+19];
			a += array[k+20];
			a += array[k+21];
			a += array[k+22];
			a += array[k+23];
			a += array[k+24];
			a += array[k+25];
			a += array[k+26];
			a += array[k+27];
			a += array[k+28];
			a += array[k+29];
			a += array[k+30];
			a += array[k+31];
*/
		}
	}
	t1 = rdtscp();
	diff = t1 - t0;

	printf("%llu\n",a);
	double bw = 4*(double)size*LOOP_TIMES/(MEGABYTE*(double)(diff - 2*LOOP_OH - READ_OH)*CYCLE);
	return bw;

}


