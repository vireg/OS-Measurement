#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define ONE p = (int**)*p;
#define TEN ONE ONE ONE ONE ONE ONE ONE ONE ONE ONE
#define HUNDRED TEN TEN TEN TEN TEN TEN TEN TEN TEN TEN
#define READ_OH 32	// Obtained from previous measurement
#define LOOP_OH 6	// Obtained from previous measurement
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

int main(int argc, char **argv, char **envp) {
	
	unsigned long def_iteration = 10000, def_stride, def_size;
	char filename[] = "mem.txt";
	//printf("Usage: ./rama.o def_iteration def_stride def_size\n");
	if (argc < 3)
	{
		printf("Usage: ./rama.o def_iteration def_stride def_size\n");
		//char filename[] = "mem.csv";
		//def_iteration = 10000;
		def_stride = 128;
		def_size = 1024*1024;
	}
	else {	
		//printf("argc = %d\n",argc);
		//def_iteration = atol(argv[1]);
		def_stride = atol(argv[1]);
		def_size = atol(argv[2]);
	}
	//printf("def_iteration = %lu, def_stride = %lu, def_size = %lu\n",def_iteration,def_stride,def_size);

	unsigned long long t0, t1, cyc;
	FILE *file;
	file = fopen(filename,"a+");
	if (!file) {
		printf("Unable to open file : %s. Exiting...\n", filename);
		exit(1);
	}

	int i,j,k;
	int** p = NULL;
	int** array = (int **) malloc(sizeof(int*) * def_size);

	for (i = def_stride; i < def_size; i += def_stride) {
		array[i] = (int*)&array[i-def_stride];
	}
	array[0] = (int*)&array[i-def_stride];

	p = array;
	
	t0 = rdtsc();
	for (k=0; k < def_iteration; k++) {
		HUNDRED;
	}
	t1 = rdtscp();
	free(array);

	cyc = t1 - t0 - READ_OH - LOOP_OH;
	double time = (double)cyc/(def_iteration*100);
	unsigned long size_KB = (def_size * sizeof(int*)) / 1024;
	fprintf(file,"%llu\n", cyc);
	printf("iter:%lu stride:%lu\tcycles:%llu\tsize:%lu\tsize:%luKB\n", def_iteration, def_stride, cyc, def_size, size_KB);
	fclose(file);

	return 0;
}


