/**
 * Program to scale and project data into 2D and find a centroid
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

long q(long n) {
    if(n<3) {
        return 1;
    }
    long qn1,qn2,q1,q2;
#pragma omp task shared(qn1)
    {
	qn1 = q(n-1);
    }
    qn2 = q(n-2);
    #pragma omp taskwait
#pragma omp task shared(q1,qn1)
    {
	q1 = q(n-qn1);
    }
    q2 = q(n-qn2);
    #pragma omp taskwait
    return q1+q2;
}
    
int main(int argc, char** argv) {
    long out;
    if(argc!=2) {
        printf("Usage: %s <integer>\nExiting...\n",argv[0]);
    }
    long n = strtol(argv[1],NULL,10);

    struct timespec start_time;
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC,&start_time);
    #pragma omp parallel
    {
	#pragma omp single nowait
	{
	    out = q(n);
	}
    }
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    long msec = (end_time.tv_sec - start_time.tv_sec)*1000 + (end_time.tv_nsec - start_time.tv_nsec)/1000000;
    
    // Print output
    printf("q(%d) = %d\n",n,out);
    printf("found in %dms\n",msec);
    
    return EXIT_SUCCESS;
}
