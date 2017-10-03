/**
 * Program to scale and project data into 2D and find a centroid
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>

/* struct to hold objects attributes */
struct phaseball {
    float x;
    float y;
    float z;
    float mass;
};

struct volume {
    float* x;
    float* y;
    float* z;
    float* mass;
    int size;
    int last;
};

// Add phaseball to a volume
void volume_append(struct volume* v, struct phaseball* o) {
    if( v->last == v->size ) {
        (v->size) += 100;
        v->x = realloc(v->x, sizeof(float)*(v->size));
	v->y = realloc(v->y, sizeof(float)*(v->size));
	v->z = realloc(v->z, sizeof(float)*(v->size));
	v->mass = realloc(v->mass, sizeof(float)*(v->size));
    }
    (v->x)[(v->last)] = o->x;
    (v->y)[(v->last)] = o->y;
    (v->z)[(v->last)] = o->z;
    (v->mass)[(v->last)] = o->mass;
    (v->last) += 1;
    return;
}
        
// Place phaseballs uniformly in a box, with mass equal to the manhattan distance
void place_uniformly(int sx, int ex, int sy, int ey, int sz, int ez, struct volume* v) {
    struct phaseball n;
    for(int i=sx; i<=ex; i++) {
        for(int j=sy; j<=ey; j++) {
            for(int k=sz; k<=ez; k++) {
                //struct phaseball* n = malloc(sizeof(struct phaseball));
                n.x = i;
                n.y = j;
                n.z = k;
                n.mass = 1;
                n.mass = fabs(n.x)+fabs(n.y)+fabs(n.z);
                volume_append(v,&n);
            }
        }
    }
}

// Projects 3D volume to 11x11 2D map and report centroid
void post_process(struct volume* v, float* cx, float* cy) {
    /*double mass_sum=0.0;
      double wx=0.0;
      double wy=0.0;*/
    
    CILK_C_REDUCER_OPADD(mass_sum, double, 0.0);
    CILK_C_REDUCER_OPADD(wx, double, 0.0);
    CILK_C_REDUCER_OPADD(wy, double, 0.0);
    
    cilk_for(int i=0; i<v->last; i++) {
	//struct phaseball* o = v->objects[i];
	REDUCER_VIEW(mass_sum) += (v->mass)[i];
	REDUCER_VIEW(wx) += (v->x)[i] * (v->mass)[i];
	REDUCER_VIEW(wy) += (v->y)[i] * (v->mass)[i];
    }
    *cx = wx.value/mass_sum.value;
    *cy = wy.value/mass_sum.value;
    
    return;
}

int main(int argc, char** argv) {
    // make a volume to store objects in
    struct volume v;
    v.size=100;
    v.last=0;
    v.x = malloc(sizeof(float)*100);
    v.y = malloc(sizeof(float)*100);
    v.z = malloc(sizeof(float)*100);
    v.mass = malloc(sizeof(float)*100);

    // Set the initial configuration
    place_uniformly(-1000,1000,-100,100,-100,100,&v);

    // Generate output
    float cx,cy;
    struct timespec start_time;
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC,&start_time);
    post_process(&v, &cx, &cy);
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    long msec = (end_time.tv_sec - start_time.tv_sec)*1000 + (end_time.tv_nsec - start_time.tv_nsec)/1000000;
    
    // Print output
    printf("Centroid at: %f,%f\n",cx,cy);
    printf("found in %dms\n",msec);
    
    return EXIT_SUCCESS;
}
