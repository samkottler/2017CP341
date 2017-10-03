#include <stdio.h>
#include <math.h>

#include "serial.h"

int main(int argc, char** argv){
    char* filename;
    if (argc > 1) filename = argv[1];
    else filename = "out.png";

    int image[WIDTH*HEIGHT];
    for (int i = 0; i<WIDTH*HEIGHT; i++) image[i] = 0xffffff;

    double b[NUM_TERMS];
    genCoefficients(b);
    
    point_t points[NUM_POINTS];
    for (int i = 0; i<NUM_POINTS; i++){
	double theta = 2*3.1415*i/NUM_POINTS;
	point_t z = (point_t){cos(theta), sin(theta)};
	point_t phiZ = (point_t){z.x,z.y};
	point_t temp = (point_t){1,0};
	invert(&z,&z);
	for (int n = 0; n<NUM_TERMS; n++){
	    //printf("%f %f\n", phiZ.x, phiZ.y);
	    phiZ.x += b[n]*temp.x;
	    phiZ.y += b[n]*temp.y;
	    double t = temp.x;
	    temp.x = z.x*temp.x-z.y*temp.y;
	    temp.y = z.x*temp.y+t*z.y;
	}
	points[i].x=phiZ.x;
	points[i].y=phiZ.y;
    }

    drawPoints(image,points);
    
    writeImage(filename, WIDTH, HEIGHT, image);
    return 0;
}

void genCoefficients(double* coefs){
    for (int n = 0; n<NUM_TERMS; n++){
	if (n == 0) coefs[n] = -0.5;
	else coefs[n] = -w(n,n+1)/n;
	printf("%f\n",coefs[n]);
    }
}

double w(int n, int m){
    if (n == 0) return 0;
    double sum = 0;
    for (int j = 0; j<=m-2; j++){
	sum+=u(0,j+1)*w(n-1,m-j-1);
    }
    sum+=u(0,m)+w(n-1,m);
    //printf("w(%d,%d)=%f\n",n,m,sum);
    return sum;
}

double u(int n, int k){
    if ((1<<n)-1 == k) return 1;
    if ((1<<n)-1 > k){
	double sum = 0;
	for (int j = 0; j<=k; j++){
	    sum+=u(n-1,j)*u(n-1,k-j);
	}
	return sum;
    }
    if ((1<<(n+1))-1>k) return 0;
    double sum = 0;
    for (int j = 1; j<=k-1; j++){
	sum+=u(n,j)*u(n,k-j);
	//printf("%d %d %f\n",n,k,sum);
    }
    sum = 0.5*(u(n+1,k)-sum);
    //printf("u(%d,%d)=%f\n",n,k,sum);
    return sum;
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
