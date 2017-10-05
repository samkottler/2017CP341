#include <stdio.h>
#include <math.h>

#include "serial.h"

int main(int argc, char** argv){
    char* filename;
    if (argc > 1) filename = argv[1];
    else filename = "out.png";

    initArrs();
    
    int image[WIDTH*HEIGHT];
    for (int i = 0; i<WIDTH*HEIGHT; i++) image[i] = 0xffffff;

    double b[NUM_TERMS];
    genCoefficients(b);

    /*
    for (int n = 0; n<NUM_TERMS/2; n++){
	for (int m = 0; m<NUM_TERMS+1; m++){
	    if (barr[n][m]==INFINITY) printf("%d,%d\n",n,m);
	}
    }
    */
    
    point_t points[NUM_POINTS];
    for (double r = 1; r<2; r+=1){
	for (int i = 0; i<NUM_POINTS; i++){
	    double theta = 2*3.1415*i/NUM_POINTS;
	    point_t z = (point_t){cos(theta)*r, sin(theta)*r};
	    //printf("z=%f+%fi\n",z.x,z.y);
	    point_t phiZ = (point_t){z.x,z.y};
	    point_t temp = (point_t){1,0};
	    invert(&z,&z);
	    for (int n =0; n<NUM_TERMS; n++){
		//printf("z^(-%d)=%f+%fi\n",n, temp.x, temp.y);
		phiZ.x += b[n]*temp.x;
		phiZ.y += b[n]*temp.y;
		double t = temp.x;
		temp.x = z.x*temp.x-z.y*temp.y;
		temp.y = z.x*temp.y+t*z.y;
	    }
	    points[i].x=phiZ.x;
	    points[i].y=phiZ.y;
	    //printf("Phi(z)=%f+%fi\n",phiZ.x,phiZ.y);
	}
    
	drawPoints(image,points);
    }
    writeImage(filename, WIDTH, HEIGHT, image);
    return 0;
}

void genCoefficients(double* coefs){
    for (int n = 0; n<NUM_TERMS; n++){
	if (n == 0) coefs[n] = -0.5;
	else coefs[n] = beta(0,n+1);
	//printf("%.10f\n",coefs[n]);
    }
}


/* adapted from http://fraktal.republika.pl/mset_jungreis.html */
double beta(int n, int m){
    //if (n<0 || m<0 || n>NUM_TERMS/2-1 || m>NUM_TERMS) printf("Bad Access: %d,%d\n",n,m);
    if (barr[n][m]!=INFINITY) return barr[n][m];
    int num = ((1<<(n+1))-1);
    double toReturn;
    if (m == 0) toReturn = 1;
    else if (n>0 && m<num) toReturn = 0;
    else{
	toReturn = beta(n+1,m) - beta(0,m-num);
	for (int i = num; i<=m-num; i++){
	    toReturn-=beta(n,i)*beta(n,m-i);
	}
	toReturn*=0.5;
    }
    barr[n][m]=toReturn;
    return toReturn;
}

double w(int n, int m){
    if (warr[n][m]!=INFINITY) return warr[n][m];
    double toReturn;
    if (n == 0) toReturn = 0;
    else{
	double sum = 0;
	for (int j = 0; j<=m-2; j++){
	    sum+=u(0,j+1)*w(n-1,m-j-1);
	}
	sum+=u(0,m)+w(n-1,m);
	toReturn = sum;
    }
    //printf("w(%d,%d)=%f\n",n,m,toReturn);
    warr[n][m] = toReturn;
    return toReturn;
}

double u(int n, int k){
    if (uarr[n][k]!=INFINITY) return uarr[n][k];
    double toReturn;
    if ((1<<n)-1 == k) toReturn = 1;
    else if ((1<<n)-1 > k){
	double sum = 0;
	for (int j = 0; j<=k; j++){
	    sum+=u(n-1,j)*u(n-1,k-j);
	}
	toReturn = sum;
    }
    else if ((1<<(n+1))-1>k) toReturn = 0;
    else{
	double sum = 0;
	for (int j = 1; j<=k-1; j++){
	    sum+=u(n,j)*u(n,k-j);
	    //printf("%d %d %f\n",n,k,sum);
	}
	toReturn = 0.5*(u(n+1,k)-sum);
    }
    //printf("u(%d,%d)=%f\n",n,k,toReturn);
    uarr[n][k]=toReturn;
    return toReturn;
}

void drawPoints(int* image, point_t* points){
    for (int i = 0; i<NUM_POINTS; i++){
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
