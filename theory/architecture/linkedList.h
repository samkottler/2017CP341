#ifndef __LINKED_H
#define __LINKED_H

typedef struct node node_t, *node_p;

struct node{
    int year;
    int stops;
    node_p next;
};

node_p listFind(node_p, int);
void printList(node_p);
void deleteList(node_p);

#endif
