#include <stdio.h>
#include <stdlib.h>

#include "linkedList.h"

node_p listFind(node_p head, int year){
    if (head->year == 0){
	node_p newNode_p = (node_p) malloc(sizeof(node_t));
	newNode_p->year = 0;
	newNode_p->stops = 0;
	newNode_p->next = NULL;
	head->year = year;
	head->next = newNode_p;
	return head;
    }
    if (head->year==year){
	return head;
    }
    return listFind(head->next, year);
}

void printList(node_p head){
    if (head->year == 0){
	return;
    }
    printf("%d had %d stops\n", head->year, head->stops);
    printList(head->next);
}

void deleteList(node_p head){
    node_p next = head->next;
    free(head);
    if (next){
	deleteList(next);
    }
}
