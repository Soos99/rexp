#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX 1000
#define FAILED NULL

//TODO: memory leak, scanning
//Problem: table-driven?

/*
    Rule 1: <E> -> <C><ET>
    Rule 2: <ET> -> |<E>  |  eps
    Rule 3: <C> -> <S><CT>
    Rule 4: <CT> -> .<C>  |  eps
    Rule 5: <S> -> <A><ST>
    Rule 6: <ST> -> *<ST> |  eps
    Rule 7: <A> -> (<E>)  |  <X>
    Rule 8: <X> -> a|b|c|...|z

    Table:
         |   0   | 1 | 2 | 3 |  4  |  5  |  6 
    |    | [a-z] | . | * | | | eps |  (  |  )
    |  E |   1   | 0 | 0 | 0 |  0  |  0  |  0
    |  C |   3   | 0 | 0 | 0 |  0  |  0  |  0
    |  ET|   0   | 0 | 0 | 2 |  2  |  0  |  0
    |  CT|   0   | 4 | 0 | 0 |  4  |  0  |  0
    |  S |   5   | 0 | 0 | 0 |  0  |  0  |  0
    |  A |   7   | 0 | 0 | 0 |  0  |  7  |  0
    |  ST|   0   | 0 | 6 | 0 |  6  |  0  |  0
    |  X |   8   | 0 | 0 | 0 |  0  |  0  |  0


*/

int parseTable[9][7] = {
    {0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0},
    {3,0,0,0,0,0,0},
    {0,0,0,2,2,0,0},
    {0,4,0,0,4,0,0},
    {5,0,0,0,0,0,0},
    {7,0,0,0,0,7,7},
    {0,0,6,0,6,0,0},
    {8,0,0,0,0,0,0}
};


typedef struct Node *Tree;
struct Node
{
    char *label;
    Tree child;
    Tree sibling;
    int indent;
};

Tree makeNode0(char *x,int indent);
Tree makeNode1(char *x,Tree t1,int indent);
Tree makeNode2(char *x,Tree t1,Tree t2,int indent);
Tree makeNode3(char *x,Tree t1,Tree t2,Tree t3, int indent);
Tree makeNode4(char *x,Tree t1,Tree t2,Tree t3,Tree t4,int indent);


void getLabel(char *x, int indent);
void printParseTree();
void printError();

Tree parseRoot;
char printing[MAX];
int indexPrinting = 0;
char *nextTerminal;
Tree stack[MAX];
int size = 0;

Tree parsing();

Tree pop();
void push(Tree curr);

Tree rule0(Tree curr);
Tree rule1(Tree curr);
Tree rule2(Tree curr);
Tree rule3(Tree curr);
Tree rule4(Tree curr);
Tree rule5(Tree curr);
Tree rule6(Tree curr);
Tree rule7(Tree curr);
Tree rule8(Tree curr);

int switchChar(char c);

void main(){
    nextTerminal = "a|b.c.c";
    parseRoot = parsing();
    if (parseRoot == FAILED){
        printError();
    }
    else {
        printParseTree();
    }
}

Tree parsing(){
    Tree root = makeNode0("E",0);
    push(root);
    while (size != 0){
        //pop curr Node
        Tree curr = pop();
        if (curr == FAILED) return FAILED;
        // check valid input
        int num = switchChar(*nextTerminal);
        if (num == -1) {
            return FAILED;
        }
        //check label
        char label1 = curr->label[0];
        char label2;
        if (label1 > 64 && label1 < 91){ //Capital Letter
            //look at label => rule number
            //follow the rule
            switch (label1)
            {
                case 'E':
                    label2 = curr->label[1];
                    if (label2 == 'T'){
                        curr = rule2(curr);
                    }
                    else {
                        curr = rule1(curr);
                    }
                    break;
                case 'S':
                    label2 = curr->label[1];
                    if (label2 == 'T'){
                        curr = rule6(curr);
                    }
                    else {
                        curr = rule5(curr);
                    }
                    break;
                case 'C':
                    label2 = curr->label[1];
                    if (label2 == 'T'){
                        curr = rule4(curr);
                    }
                    else {
                        curr = rule3(curr);
                    }
                    break;
                case 'A':
                    curr = rule7(curr);
                    break;
                case 'X':
                    curr = rule8(curr);
                    break;
                default:
                    return FAILED;
                    break;
            }
        }
        else {
            if (curr->label != "eps" && *nextTerminal != label1) return FAILED;
            else if (*nextTerminal == label1) nextTerminal++;
        }
        if (curr != FAILED) getLabel(curr->label,curr->indent);
        else return FAILED;
    }
    if (*nextTerminal != '\0') return FAILED;
    return root;
}

int switchChar(char c){
    if ((int) c > 96 && (int) c < 123) return 0;
    else if (c == '.') return 1;
    else if (c == '*') return 2;
    else if (c == '|') return 3;
    else if (c == '\0') return 4;
    else if (c == ')') return 5;
    else if (c == '(') return 6;
    else return -1;
}

Tree makeNode0(char *x, int indent){
    Tree root;

    root = (Tree) malloc(sizeof(struct Node));
    root->label = x;
    root->child = NULL;
    root->sibling = NULL;
    root->indent = indent;
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

Tree pop() {
    Tree c;
    if (size > 0){
        c = stack[size];
        size--;
    }
    return c;
}

void push(Tree c){
    if (size < MAX){
        size++;
        stack[size] = c;
    }
}

Tree rule0(Tree curr) {
    push(FAILED);
    return FAILED;
}

Tree rule1(Tree curr){
    int num = curr->indent+1;
    Tree concat = makeNode0("C",num);
    Tree et = makeNode0("ET",num);
    push(et);
    push(concat);
    return makeNode2("E",concat,et,num-1);
}

Tree rule3(Tree curr){
    int num = curr->indent+1;
    Tree star = makeNode0("S",num);
    Tree ct = makeNode0("CT",num);
    push(ct);
    push(star);
    return makeNode2("C",star,ct,num-1);
}

Tree rule2(Tree curr){
    int num = curr->indent+1;
    if (*nextTerminal == '|'){
        Tree e = makeNode0("E",num);
        Tree straight = makeNode0("|",num);
        push(e);
        push(straight);
        return makeNode2("ET",straight,e,num-1);
    }
    else {
        Tree eps = makeNode0("eps",num);
        push(eps);
        return makeNode1("ET",eps,num-1);
    }
}
Tree rule5(Tree curr){
    int num = curr->indent+1;
    Tree atomic, st;
    atomic = makeNode0("A",num);
    st = makeNode0("ST",num);
    push(st);
    push(atomic);
    return makeNode2("S",atomic,st,num-1);
}
Tree rule4(Tree curr){
    int num = curr->indent+1;
    if (*nextTerminal == '.'){
        Tree c;
        Tree dot = makeNode0(".",num);
        c = makeNode0("C",num);
        push(c);
        push(dot);
        return makeNode2("CT",dot,c,num-1);
    }
    else{
        Tree eps = makeNode0("eps",num);
        push(eps);
        return makeNode1("CT",eps,num-1);
    }
}
Tree rule7(Tree curr){
    int num = curr->indent+1;
    if (*nextTerminal == '('){
        Tree e;
        Tree open, close;
        e = makeNode0("E",num);
        open = makeNode0("(",num);
        close = makeNode0(")",num);
        push(close);
        push(e);
        push(open);
        return makeNode3("A",open,e,close,num-1);
    }
    else {
        Tree e;
        e = makeNode0("X",num);
        //printf("Checked\n");
        push(e);
        return makeNode1("A",e,num-1);
    }
}
Tree rule6(Tree curr){
    int num = curr->indent+1;
    if (*nextTerminal == '*'){
        Tree st;
        Tree star;
        st = makeNode0("ST",num);
        star = makeNode0("*",num);
        push(st);
        push(star);
        return makeNode2("ST",star,st,num-1);
    }
    else {
        Tree eps = makeNode0("eps",num);
        push(eps);
        return makeNode1("ST",eps,num-1);
    }
}
Tree rule8(Tree curr){
    char c = *nextTerminal;
    int num = curr->indent+1;
    if ((int) c > 96 && (int) c < 123){
        //nextTerminal++;
        char *input = malloc(2*sizeof(char));
        input[0] = c;
        input[1] = '\0';
        Tree in = makeNode0(input,num);
        push(in);
        return makeNode1("X",in,num-1);
    }
    else {
        //Tree c = FAILED;
        //push(c);
        return FAILED;
    }
}

void getLabel(char *x, int indent) {
    int index = 0;
    while (index < indent){
        printing[indexPrinting] = ' ';
        indexPrinting++;
        printing[indexPrinting] = ' ';
        indexPrinting++;
        index++;
        //printf("  ");
    }
    int i = 0;
    while (x[i] != '\0'){
        printing[indexPrinting] = x[i];
        indexPrinting++;
        i++;
    }
    printing[indexPrinting] = '\n';
    indexPrinting++;
    //printf("%s\n",x);
}
void printParseTree(){
    int i = 0;
    while (printing[i] != '\0'){
        printf("%c",printing[i]);
        i++;
    }
}
void printError(){
    printf("The input is not well-formed");
}

