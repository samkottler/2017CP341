#include <stdio.h>
#include <math.h>

#include "mandelbrot.h"

void genPoints(double* b, point_t* points){
    for (int i = 0; i<num_points; i++){
	double theta = 2*M_PI*i/num_points;
	point_t z = (point_t){cos(theta), sin(theta)};
	//printf("z=%f+%fi\n",z.x,z.y);
	point_t phiZ = (point_t){z.x,z.y};
	point_t temp = (point_t){1,0};
	invert(&z,&z);
	for (int n =0; n<num_terms; n++){
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
    for (int i = 0; i<num_terms+1; i++){
	for (int n = 0; n<n_max; n++){
	    int num = ((1<<(n+1))-1);
	    if (i==0){
		int mMin = ((1<<n)-1);
		for (int m = mMin; m<num && m<num_terms+1; m++){
		    if (m == 0) barr[n][m] = 1;
		    else barr[n][m] = 0;
		}		     
	    }
	    else{
		int m = num+i-1;
		if (m<num_terms+1){
		    double toReturn;
		    toReturn = barr[n+1][m] - barr[0][m-num];
		    for (int i = num; i<=m-num; i++){
			toReturn-=barr[n][i]*barr[n][m-i];
		    }
		    toReturn*=0.5;
		    barr[n][m]=toReturn;
		}
	    }
	}
    }
    for (int n = 0; n<num_terms; n++){
	coefs[n]=barr[0][n+1];
	//printf("%f\n",coefs[n]);
    }
}
