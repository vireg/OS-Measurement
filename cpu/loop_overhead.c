#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <numeric>
#include <algorithm>
using namespace std;

#define ITERATIONS 15
#define LOOP_TIMES 1000
#define READ_OH 32	// Obtained from previous measurement

int compare(const void *, const void *);
void get_elapsed_time(unsigned long *);

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
        printf("Iteration %d: loop overhead = %lu cycles\n", i, timings[i]);
    }

    double loop_max = max/LOOP_TIMES;
    double loop_min = min/LOOP_TIMES;
    double loop_range = range/LOOP_TIMES;
    double loop_avg = avg/LOOP_TIMES;
    printf("Max:\t%f\nMin:\t%f\nRange:\t%f\n",loop_max,loop_min,loop_range);
    printf("avg:\t%f cycles\n", loop_avg);
    printf("med:\t%f cycles\n", (double)timings[ITERATIONS/2]/LOOP_TIMES);
    
    return 0;
}

int compare (const void * a, const void * b) 
{
    return ( *(int*)a - *(int*)b );
}

void get_elapsed_time(unsigned long *timings) {
    unsigned long long start, end;
    unsigned int i,j;
    register unsigned cycles_low0, cycles_high0, cycles_low1, cycles_high1;

    for (i = 0; i < ITERATIONS; i++) {
    
	    //asm volatile ("cpuid\n\t" "rdtsc\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" : "=r" (cycles_high0), "=r" (cycles_low0) :: "%rax", "%rbx", "%rcx", "%rdx");
	    start = rdtsc();
	    /* code to measure */
	    for(j=0;j<LOOP_TIMES;j++);
	    
	    //asm volatile ("rdtscp\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" "cpuid\n\t" : "=r" (cycles_high1), "=r" (cycles_low1) :: "%rax", "%rbx", "%rcx", "%rdx");
	    end = rdtscp();

	    //start = cycles_high0;
	    //end = cycles_high1;
            //start = ( (start << 32) | cycles_low0);
            //end = ( (end << 32) | cycles_low1);

            timings[i] = end - start - READ_OH;
    }
    
}

