#ifndef __SERIAL_H
#define __SERIAL_H

#define WIDTH 500
#define HEIGHT 400

#define X_MIN -2.4
#define X_MAX 0.6
#define Y_MIN -1.2
#define Y_MAX 1.2

double **barr;
int num_terms;
int n_max;
int num_points;

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
