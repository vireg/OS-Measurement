#include <stdio.h>
#include <stdlib.h>
#include <numeric>
#include <algorithm>
using namespace std;

#define ITERATIONS 10
#define READ_OH 32	// Obtained from previous measurement

int compare(const void *, const void *);
void get_elapsed_time(unsigned long *, int);

int procedure_0 () {}

int procedure_1 (int a1) {
	return a1 + 1;
}

int procedure_2 (int a1, int a2) {
	return a1 + a2 + 1;
}

int procedure_3 (int a1, int a2, int a3) {
	return a1 + a2 + a3 + 1;
}

int procedure_4 (int a1, int a2, int a3, int a4) {
	return a1 + a2 + a3 + a4 + 1;
}

int procedure_5 (int a1, int a2, int a3, int a4, int a5) {
	return a1 + a2 + a3 + a4 + a5 + 1;
}

int procedure_6 (int a1, int a2, int a3, int a4, int a5, int a6) {
	return a1 + a2 + a3 + a4 + a5 + a6 + 1;
}

int procedure_7 (int a1, int a2, int a3, int a4, int a5, int a6, int a7) {
	return a1 + a2 + a3 + a4 + a5 + a6 + a7 + 1;
}

int main( int argc, const char* argv[] ){

    int i, num_args;
    unsigned long timings[ITERATIONS];

    if (argc != 2)
    {
	printf("Usage: ./proc.o num_args\n");
	return 0;
    }
    num_args = atol(argv[1]);
    get_elapsed_time(timings, num_args);

    double sum = accumulate(timings, timings+ITERATIONS, 0);
    printf("Sum:\t%f\n",sum);

    double max = (double)*max_element(timings, timings+ITERATIONS);
    double min = (double)*min_element(timings, timings+ITERATIONS);
    double range = max-min;

    double avg = sum/ITERATIONS;

    //qsort (timings, ITERATIONS, sizeof(long), compare);
    for (i = 0; i < ITERATIONS; i++) {
        printf("procedure call overhead = %lu cycles\n", timings[i]);
    }

    printf("Max:\t%f\nMin:\t%f\nRange:\t%f\n",max,min,range);
    printf("avg: %f cycles\n", avg);
    printf("med: %f cycles\n", (double)timings[ITERATIONS/2]);
    
    return 0;
}

int compare (const void * a, const void * b) 
{
    return ( *(int*)a - *(int*)b );
}

void get_elapsed_time(unsigned long *timings, int num_args) {
    unsigned long start, end;
    unsigned int i,j;
    register unsigned cycles_low0, cycles_high0, cycles_low1, cycles_high1;

    for (i = 0; i < ITERATIONS; i++) {
    
	switch (num_args)
	{
		case 0:
		    	asm volatile ("cpuid\n\t" "rdtsc\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" : "=r" (cycles_high0), "=r" (cycles_low0) :: "%rax", "%rbx", "%rcx", "%rdx");
		    	procedure_0();
		    	asm volatile ("rdtscp\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" "cpuid\n\t" : "=r" (cycles_high1), "=r" (cycles_low1) :: "%rax", "%rbx", "%rcx", "%rdx");
		    	break;
		case 1:
		    	asm volatile ("cpuid\n\t" "rdtsc\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" : "=r" (cycles_high0), "=r" (cycles_low0) :: "%rax", "%rbx", "%rcx", "%rdx");
		    	procedure_1(1);
		    	asm volatile ("rdtscp\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" "cpuid\n\t" : "=r" (cycles_high1), "=r" (cycles_low1) :: "%rax", "%rbx", "%rcx", "%rdx");
		    	break;
		case 2:
		    	asm volatile ("cpuid\n\t" "rdtsc\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" : "=r" (cycles_high0), "=r" (cycles_low0) :: "%rax", "%rbx", "%rcx", "%rdx");
		    	procedure_2(1,2);
		    	asm volatile ("rdtscp\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" "cpuid\n\t" : "=r" (cycles_high1), "=r" (cycles_low1) :: "%rax", "%rbx", "%rcx", "%rdx");
		    	break;
		case 3:
		    	asm volatile ("cpuid\n\t" "rdtsc\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" : "=r" (cycles_high0), "=r" (cycles_low0) :: "%rax", "%rbx", "%rcx", "%rdx");
		    	procedure_3(1,2,3);
		    	asm volatile ("rdtscp\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" "cpuid\n\t" : "=r" (cycles_high1), "=r" (cycles_low1) :: "%rax", "%rbx", "%rcx", "%rdx");
		    	break;
		case 4:
		    	asm volatile ("cpuid\n\t" "rdtsc\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" : "=r" (cycles_high0), "=r" (cycles_low0) :: "%rax", "%rbx", "%rcx", "%rdx");
		    	procedure_4(1,2,3,4);
		    	asm volatile ("rdtscp\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" "cpuid\n\t" : "=r" (cycles_high1), "=r" (cycles_low1) :: "%rax", "%rbx", "%rcx", "%rdx");
		    	break;
		case 5:
		    	asm volatile ("cpuid\n\t" "rdtsc\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" : "=r" (cycles_high0), "=r" (cycles_low0) :: "%rax", "%rbx", "%rcx", "%rdx");
		    	procedure_5(1,2,3,4,5);
		    	asm volatile ("rdtscp\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" "cpuid\n\t" : "=r" (cycles_high1), "=r" (cycles_low1) :: "%rax", "%rbx", "%rcx", "%rdx");
		    	break;
		case 6:
		    	asm volatile ("cpuid\n\t" "rdtsc\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" : "=r" (cycles_high0), "=r" (cycles_low0) :: "%rax", "%rbx", "%rcx", "%rdx");
		    	procedure_6(1,2,3,4,5,6);
		    	asm volatile ("rdtscp\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" "cpuid\n\t" : "=r" (cycles_high1), "=r" (cycles_low1) :: "%rax", "%rbx", "%rcx", "%rdx");
		    	break;
		case 7:
		    	asm volatile ("cpuid\n\t" "rdtsc\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" : "=r" (cycles_high0), "=r" (cycles_low0) :: "%rax", "%rbx", "%rcx", "%rdx");
		    	procedure_7(1,2,3,4,5,6,7);
		    	asm volatile ("rdtscp\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" "cpuid\n\t" : "=r" (cycles_high1), "=r" (cycles_low1) :: "%rax", "%rbx", "%rcx", "%rdx");
		    	break;
		default: printf("Unsupported\n");
			 break;
	}

	start = cycles_high0;
	end = cycles_high1;
        start = ( (start << 32) | cycles_low0);
        end = ( (end << 32) | cycles_low1);

        timings[i] = end - start - READ_OH;
    }
    
}

