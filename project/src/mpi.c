#include <stdio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <mpi.h>
#include <cilk/cilk.h>

#include "mandelbrot.h"

int commrank,commsize;

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&commsize);
    MPI_Comm_rank(MPI_COMM_WORLD,&commrank);
    
    char* filename;
    if (argc > 3) filename = argv[3];
    else filename = "out.png";
    if (argc > 3) filename = argv[3];
    else filename = "out.png";
    sscanf(argv[1],"%d",&num_terms);      
    sscanf(argv[2],"%d",&num_points);
    n_max = (int)log2(num_terms+1)+1;
    if(commrank==0)
	initArrs();

    struct timespec start_time;
    struct timespec mid_time;
    struct timespec end_time;
    long msec;
    
    int image[WIDTH*HEIGHT];
    for (int i = 0; i<WIDTH*HEIGHT; i++) image[i] = 0xffffff;

    double b[num_terms];
    point_t points[num_points];
    int shareImage[WIDTH*HEIGHT*commsize];
    
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    if (commrank==0){
	genCoefficients(b);
    }
    MPI_Bcast(b,num_terms,MPI_DOUBLE,0,MPI_COMM_WORLD);
    //MPI_Scatter(b,NUM_TERMS,MPI_DOUBLE,b,NUM_TERMS,MPI_DOUBLE,0,MPI_COMM_WORLD);
    clock_gettime(CLOCK_MONOTONIC, &mid_time);
    genPoints(b,points);
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    drawPoints(image,points);
    MPI_Gather(image,WIDTH*HEIGHT,MPI_INT,shareImage,WIDTH*HEIGHT,MPI_INT,0,MPI_COMM_WORLD);

    if (commrank==0){
	for (int y = 0; y<HEIGHT; y++){
	    for (int x = 0; x<WIDTH; x++){
		int col = 0xffffff;
		for (int n = 0; n<commsize; n++){
		    if (shareImage[WIDTH*HEIGHT*n+WIDTH*y+x] == 0) col = 0;
		}
		image[WIDTH*y + x] = col;
	    }
	}
    }
    
    if (commrank==0){
	msec = (mid_time.tv_sec - start_time.tv_sec)*1000 + (mid_time.tv_nsec - start_time.tv_nsec)/1000000;
	printf("%d coeficients generated in %dms\n", num_terms, (int)msec);
	msec = (end_time.tv_sec - mid_time.tv_sec)*1000 + (end_time.tv_nsec - mid_time.tv_nsec)/1000000;
	printf("%d points tested in %dms\n", num_points, (int)msec);
    }
    
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
    
    if (commrank==0)
	writeImage(filename, WIDTH, HEIGHT, image);
    MPI_Finalize();
    return 0;
}

void genPoints(double* b, point_t* points){
    cilk_for (int i = commrank; i<num_points; i+=commsize){
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
	cilk_for (int n = 0; n<n_max; n++){
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
    for (int i = commrank; i<num_points; i+=commsize){
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
