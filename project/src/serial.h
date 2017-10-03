#ifndef __SERIAL_H
#define __SERIAL_H

#define WIDTH 500
#define HEIGHT 400
#define NUM_POINTS 1000

#define X_MIN -2.4
#define X_MAX 0.6
#define Y_MIN -1.2
#define Y_MAX 1.2

typedef struct point{
    double x;
    double y;
} point_t, *point_p;

int writeImage(char*, int, int, int*);
void drawPoints(int*, point_t*);


#endif
