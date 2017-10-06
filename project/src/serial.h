#ifndef __SERIAL_H
#define __SERIAL_H

#define WIDTH 500
#define HEIGHT 400
#define NUM_POINTS 100000
#define NUM_TERMS 30

#define X_MIN -2.4
#define X_MAX 0.6
#define Y_MIN -1.2
#define Y_MAX 1.2

#if NUM_TERMS < 1
#define N_MAX 1
#elif NUM_TERMS < 3
#define N_MAX 2
#elif NUM_TERMS < 7
#define N_MAX 3
#elif NUM_TERMS < 15
#define N_MAX 4
#elif NUM_TERMS < 31
#define N_MAX 5
#elif NUM_TERMS < 63
#define N_MAX 6
#elif NUM_TERMS < 127
#define N_MAX 7
#elif NUM_TERMS < 255
#define N_MAX 8
#elif NUM_TERMS < 511
#define N_MAX 9
#elif NUM_TERMS < 1023
#define N_MAX 10
#elif NUM_TERMS < 2047
#define N_MAX 11
#elif NUM_TERMS < 4095
#define N_MAX 12
#else
#define N_MAX 31
#endif

double warr[NUM_TERMS+1][NUM_TERMS+2];
double uarr[NUM_TERMS+1][NUM_TERMS+2];
double barr[N_MAX][NUM_TERMS+1];
void initArrs(){
    for (int i = 0; i<N_MAX; i++){
	for (int j = 0; j<NUM_TERMS+1; j++){
	    //warr[i][j]=INFINITY;
	    //uarr[i][j]=INFINITY;
	    barr[i][j]=INFINITY;
	}
    }
}

typedef struct point{
    double x;
    double y;
} point_t, *point_p;

void invert(point_p src, point_p dest){
    double x = src->x;
    double y = src->y;
    double denom = x*x+y*y;
    dest->x = x/denom;
    dest->y = -y/denom;
}

void pointPow(point_p src, point_p dest, int exp){
    double a = 1;
    double b = 1;
    double x = src->x;
    double y = src->y;
    for (int i = 0; i<exp; i++){
	double temp = a;
	a = a*x-b*y;
	b = b*x+temp*y;
    }
    dest->x = a;
    dest->y = b;
}

int writeImage(char*, int, int, int*);
void genPoints(double*,point_t*);
void genCoefficients(double*);
double w(int, int);
double u(int, int);
double beta(int, int);
void drawPoints(int*, point_t*);


#endif
