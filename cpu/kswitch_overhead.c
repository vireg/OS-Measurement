#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <numeric>
#include <algorithm>
using namespace std;
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

#define ITERATIONS 15
#define READ_OH 32	// Obtained from previous measurement

int fd[2];

int compare(const void *, const void *);
void get_elapsed_time(double *);

int main( int argc, const char* argv[] ){

    int i, num_args;
    double timings[ITERATIONS];

    get_elapsed_time(timings);

    double sum = accumulate(timings, timings+ITERATIONS, 0);
    printf("Sum:\t%f\n",sum);

    double max = (double)*max_element(timings, timings+ITERATIONS);
    double min = (double)*min_element(timings, timings+ITERATIONS);
    double range = max-min;

    double avg = sum/ITERATIONS;

    qsort (timings, ITERATIONS, sizeof(long), compare);
    for (i = 0; i < ITERATIONS; i++) {
        printf("process creation overhead = %f us\n", timings[i]);
    }

    printf("Max:\t%f\nMin:\t%f\nRange:\t%f\n",max,min,range);
    printf("avg: %f us\n", avg);
    printf("med: %f us\n", (double)timings[ITERATIONS/2]);
    
    return 0;
}

int compare (const void * a, const void * b) 
{
    return ( *(int*)a - *(int*)b );
}

void* new_thread(void *) {
	struct timeval t2;
	gettimeofday(&t2, NULL);
	write(fd[1], (void*)&t2, sizeof(timeval));
	pthread_exit(NULL);
}

void get_elapsed_time(double *timings) {
    unsigned long start, end;
    unsigned int i;
    register unsigned cycles_low0, cycles_high0, cycles_low1, cycles_high1;
    pid_t pid;
    struct timeval t1, t2; 
    double elap;

    pthread_t thread;
    pipe(fd);

    for (i = 0; i < ITERATIONS; i++) {


	    pthread_create(&thread, NULL, new_thread, NULL);
//rdtsc();
	    gettimeofday(&t1, NULL);

	    pthread_join(thread, NULL);
	    read(fd[0], (void*)&t2, sizeof(timeval));

	    elap = (t2.tv_usec - t1.tv_usec);
	    //start = cycles_high0;
	    //end = cycles_high1;
            //start = ( (start << 32) | cycles_low0);
            //end = ( (end << 32) | cycles_low1);
	    if (elap>0)
        	timings[i] = elap;
	    else {
		printf("elap<0\n");
		timings[i] = -elap;
	    }
    }
    
}

