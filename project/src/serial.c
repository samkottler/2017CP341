#include <stdio.h>
#include <math.h>

#include "serial.h"

int main(int argc, char** argv){
    char* filename;
    if (argc > 1) filename = argv[1];
    else filename = "out.png";

    int image[WIDTH*HEIGHT];
    for (int i = 0; i<WIDTH*HEIGHT; i++) image[i] = 0xffffff;
    
    point_t points[NUM_POINTS];
    for (int i = 0; i<NUM_POINTS; i++){
	double theta = 2*3.1415*i/NUM_POINTS;
	points[i] = (point_t){cos(theta), sin(theta)};
    }

    drawPoints(image,points);
    
    writeImage(filename, WIDTH, HEIGHT, image);
    return 0;
}

void drawPoints(int* image, point_t* points){
    for (int i = 0; i<NUM_POINTS; i++){
	double x = points[i].x;
	double y = points[i].y;
	int imgx = (int)((x-X_MIN)/(X_MAX-X_MIN)*WIDTH);
	int imgy = (int)((y-Y_MIN)/(Y_MAX-Y_MIN)*HEIGHT);
	//printf("%d %d %f %f\n", (int)((x-X_MIN)/(X_MAX-X_MIN)*WIDTH), imgy,x,y);
	if (imgx<WIDTH && imgx>=0 &&  imgy<HEIGHT && imgy>=0)
	    image[imgy*WIDTH+imgx] = 0;
    }
}
