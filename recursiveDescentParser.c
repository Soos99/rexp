#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "recursiveDescentParser.h"

//TODO: memory leak, scanning
//Concern: |eps?

Tree parseTree;
char *start, *nextTerminal;

void tryRecursiveDescentParser(){
    //nextTerminal = "a.b.c*";
    start = malloc(sizeof(char)*256);
    bool flag = true;
    printf("Trying Recursive Descent Parser...\n");
    while (flag){
        printf("      Enter expression here (\"quit\" to quit and no more than 255 characters):");
        char input[256];
        scanf("%255s",input);
        if (strcmp(input,"quit") == 0){
            flag = false;
        }
        else {
            printf("Result for \"%s\":\n \n", input);
            nextTerminal = start;
            strcpy(nextTerminal,input);
            parseTree = parseExpress();
            if (*nextTerminal != '\0' && parseTree == FAILED){
                printError();
            }
            else {
                printTree(parseTree,0);
            }
            freeTree(parseTree);
        }
        printf("\n");
    }
    free(start);
}

Tree buildRecursiveDescentParse(char *input) {
    nextTerminal = input;
    parseTree = parseExpress();
    return parseTree;
}

Tree parseExpress(){
    Tree concat, et;
    concat = parseConcat();
    if (concat != FAILED){
        et = parseET();

        if (et != FAILED){
            return makeNode2("E",concat,et,0);
        }
        else {
            return FAILED;
        }
    }
    else {
        return FAILED;
    }
}

Tree parseConcat(){
    Tree star, ct;
    star = parseStar();
    if (star != FAILED){
        ct = parseCT();
        if (ct != FAILED){
            return makeNode2("C",star,ct,0);
        }
        else {
            return FAILED;
        }
    }
    else {
        return FAILED;
    }
}
Tree parseET(){
    Tree t;
    if (*nextTerminal == '|'){
        nextTerminal++;
        t = parseExpress();
        if (t != FAILED){
            return makeNode2("ET",makeNode0("|",0),t,0);
        }
        else {
            return FAILED;
        }
    }
    else {
        return makeNode1("ET",makeNode0("eps",0),0);
    }
}
Tree parseStar(){
    Tree atomic, st;
    atomic = parseAtomic();
    if (atomic != FAILED){
        st = parseST();
        if (st != FAILED){
            return makeNode2("S",atomic,st,0);
        }
        else {
            return FAILED;
        }
    }
    else {
        return FAILED;
    }
}
Tree parseCT(){
    Tree t;
    if (*nextTerminal == '.'){
        nextTerminal++;
        t = parseConcat();
        if (t != FAILED){
            return makeNode2("CT",makeNode0(".",0),t,0);
        }
        else {
            return FAILED;
        }
    }
    else{
        return makeNode1("CT",makeNode0("eps",0),0);
    }
}
Tree parseAtomic(){
    Tree t;
    if (*nextTerminal == '('){
        nextTerminal++;
        t = parseExpress();
        if (t != FAILED){
            if (*nextTerminal == ')'){
                nextTerminal++;
                return makeNode3("A",makeNode0("(",0),t,makeNode0(")",0),0);
            }
            else {
                return FAILED;
            }
        }
        else {
            return FAILED;
        }
    }
    else {
        t = parseInput();
        if (t != FAILED){
            return makeNode1("A",t,0);
        }
        else {
            return FAILED;
        }
    }
}
Tree parseST(){
    Tree t;
    if (*nextTerminal == '*'){
        nextTerminal++;
        t = parseST();
        if (t != FAILED){
            return makeNode2("ST",makeNode0("*",0),t,0);
        }
        else {
            return FAILED;
        }
    }
    else {
        return makeNode1("ST",makeNode0("eps",0),0);
    }
}
Tree parseInput(){
    char c = *nextTerminal;
    if ((int) c > 96 && (int) c < 123){
        nextTerminal++;
        char *input = malloc(2*sizeof(char));
        input[0] = c;
        input[1] = '\0';
        Tree leaf = makeNode0(input,0);
        leaf->freeable = true;
        return makeNode1("X",leaf,0);
    }
    else {
        return FAILED;
    }
}

// Tree B(){
//     Tree firstB, secondB;

//     if (*nextTerminal == '('){
//         nextTerminal++;
//         firstB = B();
//         if (firstB != FAILED && *nextTerminal == ')'){
//             nextTerminal++;
//             secondB = B();
//             if (secondB == FAILED) return FAILED;
//             else {
//                 return makeNode4("E",makeNode0("("),firstB,makeNode0(")"),secondB);
//             }
//         }
//         else {
//             return FAILED;
//         }
//     }
//     else {
//         return makeNode1("S",makeNode0("B"));
//     }
// }

void printLabel(char *x, int indent) {
    int index = 0;
    while (index < indent){
        printf("  ");
        index++;
    }
    printf("%s\n", x);
}
void printTree(Tree root, int indent){
    if (root == FAILED) return;
    printLabel(root->label,indent);
    int num = indent+1;
    printTree(root->child,num);
    printTree(root->sibling,indent);
    return;
}




