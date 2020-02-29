#ifndef _Tree_h
#define _Tree_h

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Node *Tree;
struct Node {
    char *label;
    Tree child;
    Tree sibling;
    int indent;
    bool freeable;
};

Tree makeNode0(char *x, int indent);
Tree makeNode1(char *x, Tree t, int indent);
Tree makeNode2(char *x, Tree t1, Tree t2,int indent);
Tree makeNode3(char *x, Tree t1, Tree t2, Tree t3, int indent);
Tree makeNode4(char *x, Tree t1, Tree t2, Tree t3, Tree t4, int indent);
void freeTree(Tree tree);
void printError();

#endif