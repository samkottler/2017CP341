#include <omp.h>
#include <stdio.h>

int main(int argc, char** argv) {
    size_t T=8;
    int t_hello[T];
    int i=0;
    omp_lock_t lock;
    omp_init_lock(&lock);
    for(i=0;i<T;i++) {
        t_hello[i]=-1;
    }
    i=0;
    #pragma omp parallel num_threads(8)
    {
        omp_set_lock(&lock);
        t_hello[i]=omp_get_thread_num();
	i++;
	omp_unset_lock(&lock);
    }

    for(i=0;i<T;i++) {
        printf("Hello from %d\n",t_hello[i]);
    }

    return 0;
}
