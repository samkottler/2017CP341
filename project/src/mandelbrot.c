#include <stdio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#include "mandelbrot.h"

int main(int argc, char** argv){
    char* filename;
    if (argc > 3) filename = argv[3];
    else filename = "out.png";
    sscanf(argv[1],"%d",&num_terms);
    sscanf(argv[2],"%d",&num_points);
    n_max = (int)log2(num_terms+1)+1;
    
    initArrs();

    struct timespec start_time;
    struct timespec mid_time;
    struct timespec end_time;
    long msec;
    
    int image[WIDTH*HEIGHT];
    for (int i = 0; i<WIDTH*HEIGHT; i++) image[i] = 0xffffff;

    double b[num_terms];
    point_t points[num_points];
    
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    genCoefficients(b);
    clock_gettime(CLOCK_MONOTONIC, &mid_time);
    genPoints(b,points);
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    msec = (mid_time.tv_sec - start_time.tv_sec)*1000 + (mid_time.tv_nsec - start_time.tv_nsec)/1000000;
    printf("%d coeficients generated in %dms\n", num_terms, (int)msec);
    msec = (end_time.tv_sec - mid_time.tv_sec)*1000 + (end_time.tv_nsec - mid_time.tv_nsec)/1000000;
    printf("%d points tested in %dms\n", num_points, (int)msec);
    
#ifdef CHECK_ARR
    printf("n\\m");
    for (int m = 0; m<NUM_TERMS+1; m++) printf("%3d",m);
    printf("\n"); 
    for (int n = 0; n<N_MAX; n++){
	printf("%2d ",n);
        for (int m = 0; m<NUM_TERMS+1; m++){
            if (barr[n][m]==INFINITY) printf("[ ]");
	    else printf("[X]");
        }
	printf("\n");
    }
#endif
    
    drawPoints(image,points);
    writeImage(filename, WIDTH, HEIGHT, image);
    return 0;
}

void initArrs(){
    barr = (double **) malloc(sizeof(double*)*(n_max));
    for (int i = 0; i<n_max; i++){
	barr[i]=(double *)malloc(sizeof(double)*(num_terms+1));
	for (int j = 0; j<num_terms+1; j++){
	    barr[i][j]=INFINITY;
	}
    }
}

void invert(point_p src, point_p dest){
    double x = src->x;
    double y = src->y;
    double denom = x*x+y*y;
    dest->x = x/denom;
    dest->y = -y/denom;
}

void drawPoints(int* image, point_t* points){
    for (int i = 0; i<num_points; i++){
	double x = points[i].x;
	double y = points[i].y;
	int imgx = (int)((x-X_MIN)/(X_MAX-X_MIN)*WIDTH);
	int imgy = (int)((y-Y_MIN)/(Y_MAX-Y_MIN)*HEIGHT);
	//printf("%d %d %f %f\n", (int)((x-X_MIN)/(X_MAX-X_MIN)*WIDTH), imgy,x,y);
	if (imgx<WIDTH && imgx>=0 &&  imgy<HEIGHT && imgy>=0){
	    image[imgy*WIDTH+imgx] = 0;
	}
    }
}
