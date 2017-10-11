#ifndef __SERIAL_H
#define __SERIAL_H

#define WIDTH 500
#define HEIGHT 400
#define NUM_POINTS 300000
#define NUM_TERMS 20000

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
#elif NUM_TERMS < 8191
#define N_MAX 13
#elif NUM_TERMS < 16383
#define N_MAX 14
#elif NUM_TERMS < 32767
#define N_MAX 15
#else
#define N_MAX 31
#endif

double barr[N_MAX][NUM_TERMS+1];

typedef struct point{
    double x;
    double y;
} point_t, *point_p;

void initArrs();
void invert(point_p src, point_p dest);
int writeImage(char*, int, int, int*);
void genPoints(double*,point_t*);
void genCoefficients(double*);
double beta(int, int);
void drawPoints(int*, point_t*);


#endif
