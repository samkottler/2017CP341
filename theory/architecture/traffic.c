#include <stdio.h>
#include <stdlib.h>

#include "traffic.h"
#include "linkedList.h"

int parseYear(char* text){
    int year = -1;
    sscanf(text+6, "%d", &year);
    return year;
}

int main(int argc, char** argv){
    if (argc<2){
	printf("Usage: %s <traffic data>",argv[0]);
	return 0;
    }

    FILE* f = fopen(argv[1],"r");
    
    node_p outList = (node_p) malloc(sizeof(outList));
    
    char line[1024];
    fgets(line,1024,f);

    char* l = fgets(line,1025,f);
    while (l!=NULL){
	int year = parseYear(l);
	node_p node = listFind(outList, year);
	node->stops++;
	//if (l == NULL) printf("done");
	//printList(outList);
	l = fgets(line,1024,f);
    }
    
    fclose(f);

    printList(outList);

    deleteList(outList);
    
}
