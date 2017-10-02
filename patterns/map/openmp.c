/**
 * This program takes in a hash and attempts to find a matching 8 digit pin
 */

// sprintf, printf 
#include <stdio.h>

// strlen, strncmp
#include <string.h>

// exit value 
#include <stdlib.h>

// md5 hash function
#include <openssl/md5.h>

// clock measurements
#include <time.h>

#include <omp.h>

// valid pin characters
const char* chars="0123456789";

// tests if a hash matches a candidate password
int test(const char* passhash, const char* passcandidate) {
    unsigned char digest[MD5_DIGEST_LENGTH]; // hash storage
    
    // Compute the hash
    MD5((unsigned char*)passcandidate, strlen(passcandidate), digest);
    
    // Convert the hash to a base 16 string
    char mdString[34];
    mdString[33]='\0';
    for(int i=0; i<16; i++) {
        sprintf(&mdString[i*2], "%02x", (unsigned char)digest[i]);
    }
    
    // return the value of the comparison
    return strncmp(passhash, mdString, strlen(passhash));
}

// maps a numeric PIN to a string
void genpass(long passnum, char* passbuff) {
    passbuff[8]='\0'; // enforce null termination
    int charidx; // character within array of valid characters
    
    // determine the "base" for the pin string
    int symcount=strlen(chars);
    
    // perform the conversion
    for(int i=7; i>=0; i--) {
        charidx=passnum%symcount;
        passnum=passnum/symcount;
        passbuff[i]=chars[charidx];
    }
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: %s <password hash>\n",argv[0]);
        return 1;
    }
    
    char passmatch[9]; // buffer for the matched password

    // variables for the timer
    struct timespec start_time;
    struct timespec end_time;
    long msec;
    
    clock_gettime(CLOCK_MONOTONIC,&start_time);
    // While a match has not been found, search
    int notfound=1;
    #pragma omp parallel
    {
	struct timespec thread_start,thread_end;
	clock_gettime(CLOCK_MONOTONIC, &thread_start);
	int threadNum = omp_get_thread_num();
	int totalThreads = omp_get_num_threads();
	char thispassmatch[9];
	for (long currpass = threadNum; notfound; currpass+=totalThreads){
	    genpass(currpass,thispassmatch);
	    if (!test(argv[1],thispassmatch)){
		notfound=0;
		for (int i = 0; i<9; i++) passmatch[i]=thispassmatch[i];
	    }
	} 
	clock_gettime(CLOCK_MONOTONIC, &thread_end);
	long ttime = (thread_end.tv_sec-thread_start.tv_sec)*1000 + (thread_end.tv_nsec-thread_start.tv_nsec)/1000000;
	//printf("Thread %d took %dms\n", threadNum,ttime);
    }
    clock_gettime(CLOCK_MONOTONIC,&end_time);

    // convert the time to elapsed milliseconds
    msec = (end_time.tv_sec - start_time.tv_sec)*1000 + (end_time.tv_nsec - start_time.tv_nsec)/1000000;

    printf("found: %s in %dms\n",passmatch,msec);
    return EXIT_SUCCESS;
}

