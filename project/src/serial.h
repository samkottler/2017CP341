#ifndef __SERIAL_H
#define __SERIAL_H

#define WIDTH 500
#define HEIGHT 400
#define NUM_POINTS 100000
#define NUM_TERMS 50

#define X_MIN -2.4
#define X_MAX 0.6
#define Y_MIN -1.2
#define Y_MAX 1.2

double warr[NUM_TERMS+1][NUM_TERMS+2];
double uarr[NUM_TERMS+1][NUM_TERMS+2];
void initArrs(){
    for (int i = 0; i<NUM_TERMS+1; i++){
	for (int j = 0; j<NUM_TERMS+2; j++){
	    warr[i][j]=INFINITY;
	    uarr[i][j]=INFINITY;
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
void genCoefficients(double*);
double w(int, int);
double u(int, int);
void drawPoints(int*, point_t*);


#endif
