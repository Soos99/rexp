#include <stdlib.h>
#include <stdio.h>
#include "Tree.h"

Tree makeNode0(char *x, int indent){
    Tree root;

    root = (Tree) malloc(sizeof(struct Node));
    root->label = x;
    root->child = NULL;
    root->sibling = NULL;
    root->indent = indent;
    root->freeable = false;
    return root;
}

Tree makeNode1(char *x, Tree t, int indent){
    Tree root;

    root = makeNode0(x,indent);
    root->child = t;
    return root;
}

Tree makeNode2(char *x, Tree t1, Tree t2,int indent){
    Tree root;

    root = makeNode1(x,t1,indent);
    t1->sibling = t2;
    return root;
}

Tree makeNode3(char *x, Tree t1, Tree t2, Tree t3, int indent){
    Tree root;

    root = makeNode1(x,t1,indent);
    t1->sibling = t2;
    t2->sibling = t3;
    return root;
}

Tree makeNode4(char *x, Tree t1, Tree t2, Tree t3, Tree t4, int indent){
    Tree root;

    root = makeNode1(x,t1,indent);
    t1->sibling = t2;
    t2->sibling = t3;
    t3->sibling = t4;
    return root;
}

void freeTree(Tree tree) {
    if (tree->child)
        freeTree(tree->child);
    if (tree->sibling)
        freeTree(tree->sibling);
    if (tree->freeable)
        free(tree->label);
    free(tree);
}

void printError(){
    printf("The input is not well-formed\n");
}