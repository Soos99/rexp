#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "convertToExpressionTree.h"
#include "recursiveDescentParser.h"

Tree nodeE(Tree node);
Tree nodeC(Tree node);
Tree nodeS(Tree node);
Tree nodeST(Tree node);
Tree nodeA(Tree node);
Tree nodeX(Tree node);

char* concat(char* a, char* b);


void tryConvert() {
    bool flag = true;
    printf("-------------------------\n");
    printf("Trying Convert from parse tree to expression tree...\n");
    while (flag){
        printf("\tEnter expression here (\"quit\" to quit and no more than 255 characters):");
        char input[256];
        scanf("%255s",input);
        if (strcmp(input,"quit") == 0){
            flag = false;
        }
        else {
            Tree parseTree = buildRecursiveDescentParse(input);
            if (parseTree == NULL) continue;
            printf("Parse Tree for input %s:\n \n", input);
            printTree(parseTree, 0);
            printf("\n \n");

            printf("Result for \"%s\":\n \n", input);
            Tree expressionTree = convertToExpressionTree(parseTree);
            printExpressionTree(expressionTree);
            printf("\n");

            freeTree(parseTree);
            freeTree(expressionTree);
        }
        printf("\n");
    }
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
    if (strcmp(node->child->label, "(") == 0)
        return nodeE(node->child->sibling);
    else {
        return nodeX(node->child);
    }
}

Tree nodeX(Tree node) {
    // printf("Visit X\n");
    
    Tree letter = node->child;
    //char *atomic = "ATOMIC ";
    char* trueLabel = concat("ATOMIC ",letter->label);
    Tree expression = makeNode0(trueLabel, 0);
    expression->freeable = true;
    return expression;
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