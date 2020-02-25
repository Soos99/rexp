#include <stdlib.h>
#include <stdio.h>

//TODO: memory leak, scanning
//Concern: |eps?

#define FAILED NULL

typedef struct Node *Tree;
struct Node
{
    char *label;
    Tree child;
    Tree sibling;
};

Tree makeNode0(char *x);
Tree makeNode1(char *x,Tree t1);
Tree makeNode2(char *x,Tree t1,Tree t2);
Tree makeNode3(char *x,Tree t1,Tree t2,Tree t3);
Tree makeNode4(char *x,Tree t1,Tree t2,Tree t3,Tree t4);

Tree parseExpress();
Tree parseConcat();
Tree parseET();
Tree parseStar();
Tree parseCT();
Tree parseAtomic();
Tree parseST();
Tree parseInput();

void printLabel(char *x, int indent);
void printTree(Tree root, int indent);
void printError();

Tree parseTree;
char *nextTerminal;

void main(){
    nextTerminal = "a|b.c.*";
    parseTree = parseExpress();
    if (*nextTerminal == '\0' && parseTree != FAILED) printTree(parseTree,0);
    else {
        printError();
    }
}

Tree makeNode0(char *x){
    Tree root;

    root = (Tree) malloc(sizeof(struct Node));
    root->label = x;
    root->child = NULL;
    root->sibling = NULL;
    return root;
}

Tree makeNode1(char *x, Tree t){
    Tree root;

    root = makeNode0(x);
    root->child = t;
    return root;
}

Tree makeNode2(char *x, Tree t1, Tree t2){
    Tree root;

    root = makeNode1(x,t1);
    t1->sibling = t2;
    return root;
}

Tree makeNode3(char *x, Tree t1, Tree t2, Tree t3){
    Tree root;

    root = makeNode1(x,t1);
    t1->sibling = t2;
    t2->sibling = t3;
    return root;
}

Tree makeNode4(char *x, Tree t1, Tree t2, Tree t3, Tree t4){
    Tree root;

    root = makeNode1(x,t1);
    t1->sibling = t2;
    t2->sibling = t3;
    t3->sibling = t4;
    return root;
}

Tree parseExpress(){
    Tree concat, et;
    concat = parseConcat();
    if (concat != FAILED){
        et = parseET();

        if (et != FAILED){
            return makeNode2("E",concat,et);
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
            return makeNode2("C",star,ct);
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
            return makeNode2("ET",makeNode0("|"),t);
        }
        else {
            return FAILED;
        }
    }
    else {
        return makeNode1("ET",makeNode0("eps"));
    }
}
Tree parseStar(){
    Tree atomic, st;
    atomic = parseAtomic();
    if (atomic != FAILED){
        st = parseST();
        if (st != FAILED){
            return makeNode2("S",atomic,st);
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
            return makeNode2("CT",makeNode0("."),t);
        }
        else {
            return FAILED;
        }
    }
    else{
        return makeNode1("CT",makeNode0("eps"));
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
                return makeNode3("A",makeNode0("("),t,makeNode0(")"));
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
            return makeNode1("A",t);
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
            return makeNode2("ST",makeNode0("*"),t);
        }
        else {
            return FAILED;
        }
    }
    else {
        return makeNode1("ST",makeNode0("eps"));
    }
}
Tree parseInput(){
    char c = *nextTerminal;
    if ((int) c > 96 && (int) c < 123){
        nextTerminal++;
        char *input = malloc(2*sizeof(char));
        input[0] = c;
        input[1] = '\0';
        return makeNode1("X",makeNode0(input));
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
void printError(){
    printf("The input is not well-formed");
}




