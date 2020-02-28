#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "convertToExpressionTree.h"
#include "recursiveDescentParser.h"

/**
 * TODO: add atomic before leter
 * 
 * C ham loz deo concat string duoc!!!!!!!!!
 * */

Tree nodeE(Tree node);
Tree nodeC(Tree node);
Tree nodeS(Tree node);
Tree nodeST(Tree node);
Tree nodeA(Tree node);
Tree nodeX(Tree node);

char* concat(char* a, char* b);

void tryConvert() {
    Tree parseTree;
    char *input = "a|b.c*";

    parseTree = buildRecursiveDescentParse(input);
    printTree(parseTree, 0);

    printf("\n \n");

    Tree expressionTree = convertToExpressionTree(parseTree);
    printExpressionTree(expressionTree);
} 

Tree convertToExpressionTree(Tree root) {
    return nodeE(root);
}

Tree nodeE(Tree node) {
    // printf("Visit E\n");
    Tree c = node->child;
    Tree et = c->sibling;

    if (strcmp(et->child->label, "eps") == 0)
        return nodeC(c);

    Tree uni = et->child;
    Tree e = uni->sibling;

    Tree left = nodeC(c);
    Tree right = nodeE(e);
    Tree expression = makeNode2("UNION", left, right, 0);

    return expression; 
}

Tree nodeC(Tree node) {
    // printf("Visit C\n");
    Tree s = node->child;
    Tree ct = s->sibling;

    if (strcmp(ct->child->label, "eps") == 0)
        return nodeS(s);

    Tree dot = ct->child;
    Tree c = dot->sibling;

    Tree left = nodeS(s);
    Tree right = nodeC(c);
    Tree expression = makeNode2("CONCAT", left, right, 0);

    return expression;
}

Tree nodeS(Tree node) {
    // printf("Visit S\n");
    Tree a = node->child;
    Tree st = a->sibling;

    if (strcmp(st->child->label, "eps") == 0)
        return nodeA(a);

    Tree expression = nodeST(st);
    for (Tree temp = expression; ; temp = temp->child)
        if (temp->child == NULL) {
            temp->child = nodeA(a);
            break;
        }

    return expression;
}

Tree nodeST(Tree node) {
    // printf("Visit ST\n");
    Tree star = node->child;
    Tree st = star->sibling;

    if (strcmp(st->child->label, "eps") == 0)
        return makeNode0("CLOSURE", 0);

    Tree expression = makeNode1("CLOSURE", nodeST(st), 0); 

    return expression;
}

Tree nodeA(Tree node) {
    // printf("Visit A\n");
    if (strcmp(node->child->label, "E") == 0)
        return nodeE(node->child);
    else {
        return nodeX(node->child);
    }
}

Tree nodeX(Tree node) {
    // printf("Visit X\n");
    
    Tree letter = node->child;
    //char *atomic = "ATOMIC ";
    char* trueLabel = concat("ATOMIC ",letter->label);
    return makeNode0(trueLabel, 0);
}

void printExpressionTree(Tree root) {
    printf("(%s", root->label);

    for (Tree node = root->child; node != NULL; node = node->sibling) {
        printf(" ");
        printExpressionTree(node);
    }

    printf(")");
}

char* concat(char* a, char* b){
    char* result;
    int size1 = 0;
    int size2 = 0;
    while (a[size1] != '\0'){
        size1++;
    }
    while (b[size2] != '\0'){
        size2++;
    }
    int totalsize = size1+size2+1;
    result = (char*) malloc(totalsize*sizeof(char));
    int index = 0;
    while (index < totalsize-1){
        if (index < size1){
            result[index] = a[index];
        }
        else {
            result[index] = b[index-size1];
        }
        index++;
    }
    result[totalsize-1] = '\0';
    return result;
}