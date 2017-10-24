#include <stdio.h>
#include <stdlib.h>
#include <numeric>
#include <algorithm>
using namespace std;

#define ITERATIONS 1000

int compare(const void *, const void *);
void get_elapsed_time(unsigned long *);

int main(void)
{
    int i;
    unsigned long timings[ITERATIONS];

    get_elapsed_time(timings);

    double sum = accumulate(timings, timings+ITERATIONS, 0);
    printf("Sum:\t%f\n",sum);

    double max = (double)*max_element(timings, timings+ITERATIONS);
    double min = (double)*min_element(timings, timings+ITERATIONS);
    double range = max-min;

    double avg = sum/ITERATIONS;

    //qsort (timings, ITERATIONS, sizeof(long), compare);
    for (i = 0; i < ITERATIONS; i++) {
        printf("reading time overhead = %lu cycles\n", timings[i]);
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

void get_elapsed_time(unsigned long *timings) {
    unsigned long start, end;
    unsigned int i;
    register unsigned cycles_low0, cycles_high0, cycles_low1, cycles_high1;

    for (i = 0; i < ITERATIONS; i++) {
    
	    asm volatile ("cpuid\n\t"
			  "rdtsc\n\t"
			  "mov %%edx, %0\n\t"
			  "mov %%eax, %1\n\t"
			  : "=r" (cycles_high0), "=r" (cycles_low0)
			  :: "%rax", "%rbx", "%rcx", "%rdx");

	    /* code to measure */

	    asm volatile ("rdtscp\n\t"
			  "mov %%edx, %0\n\t"
			  "mov %%eax, %1\n\t"
			  "cpuid\n\t"
			  : "=r" (cycles_high1), "=r" (cycles_low1)
			  :: "%rax", "%rbx", "%rcx", "%rdx");

	    start = cycles_high0;
	    end = cycles_high1;
            start = ( (start << 32) | cycles_low0);
            end = ( (end << 32) | cycles_low1);

            timings[i] = end - start;
    }
    
}

