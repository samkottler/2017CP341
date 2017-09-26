#include <stdio.h>

int fib(int n){
    if (n<2) return 1;
    return fib(n-1)+fib(n-2);
}

int main(int argc, char** argv){
    for(int i = 0; i<5; i++)
	printf("%d\n",fib(i));
    return 0;
}
