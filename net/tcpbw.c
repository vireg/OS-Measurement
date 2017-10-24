#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

#define NUM_BYTES 65536
#define RADDR "127.0.0.1"
#define RPORT 2000
#define ITER 10
#define CYCLE (1/(2.6*1000000000))

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

int compare(const void *, const void *);
unsigned long get_rtt();

int main(void)
{
    int i;
    double* rttime = malloc(ITER*sizeof(double));

    for (i=0;i<ITER;i++) {
        rttime[i] = (double)get_rtt()*CYCLE*1000;
	printf("%lf\n",rttime[i]);
    }

    qsort (rttime, ITER, sizeof(double), compare);

    printf("min:\t%lf ms\n", rttime[0]);
    printf("max:\t%lf ms\n", rttime[ITER-1]);
    printf("med:\t%lf ms\n", rttime[ITER/2]);
    
    return 0;
}

int compare (const void * a, const void * b) 
{
    return ( *(int*)a - *(int*)b );
}

unsigned long get_rtt() {
    	int i, num_bytes_read, num_bytes_sent;
    	int fd;
    	struct addrinfo ai;
    	struct in_addr raddr;
    	struct sockaddr_in remote_addr;
    	char send_data[NUM_BYTES] = {0};
    	char recv_data[NUM_BYTES];
    	uint64_t start=0,end=0,total_time=0,diff=0;

    	memset(&ai, 0, sizeof(ai));
    	memset(&remote_addr, 0, sizeof(remote_addr));
	
    	ai.ai_family = AF_INET;
    	ai.ai_socktype = SOCK_STREAM;

    	raddr.s_addr = inet_addr(RADDR);

    	remote_addr.sin_family = AF_INET;
    	remote_addr.sin_port = htons(RPORT);
    	remote_addr.sin_addr = raddr;

        fd = socket(ai.ai_family, ai.ai_socktype, ai.ai_protocol);
	connect(fd, (struct sockaddr *) &remote_addr, sizeof(remote_addr));

	//printf("Sending data...\n");
	start = rdtsc();
        num_bytes_sent = send(fd, send_data, NUM_BYTES, 0);
        //num_bytes_read = recv(fd, recv_data, NUM_BYTES, 0);
	end = rdtscp();
        close(fd);

	diff = end - start;
        //printf("Sent %d bytes\n", num_bytes_sent);

    return diff;
}



