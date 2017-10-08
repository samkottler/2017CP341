#include <stdio.h>
#include <math.h>
#include <cilk/cilk.h>

#include "mandelbrot.h"

void genPoints(double* b, point_t* points){
    cilk_for (int i = 0; i<NUM_POINTS; i++){
	double theta = 2*M_PI*i/NUM_POINTS;
	point_t z = (point_t){cos(theta), sin(theta)};
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
}

void genCoefficients(double* coefs){
    cilk_for (int n = 0; n<NUM_TERMS; n++){
	coefs[n] = beta(0,n+1);
	//printf("%.10f\n",coefs[n]);
    }
}

/* adapted from http://fraktal.republika.pl/mset_jungreis.html */
double beta(int n, int m){
    //if (n<0 || m<0 || n>N_MAX || m>NUM_TERMS) printf("Bad Access: %d,%d\n",n,m);
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
