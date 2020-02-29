#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tableDrivenParser.h"

#define MAX 1000
#define FAILED NULL

//TODO: memory leak

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
    |  ET|   2   | 2 | 2 | 2 |  2  |  2  |  2
    |  CT|   4   | 4 | 4 | 4 |  4  |  4  |  4
    |  S |   5   | 0 | 0 | 0 |  0  |  0  |  0
    |  A |   7   | 0 | 0 | 0 |  0  |  7  |  0
    |  ST|   6   | 6 | 6 | 6 |  6  |  6  |  6
    |  X |   8   | 0 | 0 | 0 |  0  |  0  |  0


*/

Tree parseRoot;
char printing[MAX];
int indexPrinting = 0;
char *nextTerminal;
Tree stack[MAX];
int size = 0;
int** parseTable;

void createParseTable();
int switchChar(char c);
int getRow(char* c);
int getRule(int row, int col);
Tree chooseRule(Tree curr, int rule);

Tree ruleNegative(Tree curr);
Tree rule1(Tree curr);
Tree rule2(Tree curr);
Tree rule3(Tree curr);
Tree rule4(Tree curr);
Tree rule5(Tree curr);
Tree rule6(Tree curr);
Tree rule7(Tree curr);
Tree rule8(Tree curr);

void freeTable(int **parseTable);


void tryTableDrivenParser(){
    nextTerminal = (char*) malloc(sizeof(char)*256);
    createParseTable();
    bool flag = true;
    printf("Trying Table Driven Parser...\n");
    while (flag){
        printf("      Enter expression here (\"quit\" to quit and no more than 255 characters):");
        char input[256];
        scanf("%255s",input);
        if (strcmp(input,"quit") == 0){
            flag = false;
        }
        else {
            printf("Result for \"%s\":\n \n", input);
            strcpy(nextTerminal,input);
            parseRoot = parsing();
            if (parseRoot == FAILED){
                printError();
            }
            else {
                printParseTree();
            }
            size = 0;
        }
    }
    free(nextTerminal);
    freeTable(parseTable);
    freeTree(parseRoot);
}

Tree parsing(){
    Tree root = makeNode0("E",0);
    push(root);
    while (size != 0){
        //pop curr Node
        Tree curr = pop();
        if (curr == FAILED) return FAILED;
        //check valid input and get column
        int col = switchChar(*nextTerminal);
        if (col == -1) {
            return FAILED;
        }
        //get row by label
        int row = getRow(curr->label);
        int rule = getRule(row,col);
        if (row > 0){ //Capital Letter => label
            //look at position (row,col) => rule number
            //follow the rule
            curr = chooseRule(curr,rule);
        }
        else { //matching (terminal node)
            if (strcmp(curr->label, "eps") != 0 && *nextTerminal != curr->label[0]) return FAILED;
            else if (*nextTerminal == curr->label[0]) nextTerminal++;
        }
        if (curr != FAILED) {
            getLabel(curr->label,curr->indent);
            //freeTree(curr);
        }
        else {
            //freeTree(curr);
            return FAILED;
        }
    }
    //freeTree(curr);
    if (*nextTerminal != '\0') return FAILED;
    return makeNode0("E",0);
}

/* 
    parseTable = {
        {0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0},
        {3,0,0,0,0,0,0},
        {2,2,2,2,2,2,2},
        {4,4,4,4,4,4,4},
        {5,0,0,0,0,0,0},
        {7,0,0,0,0,7,0},
        {6,6,6,6,6,6,6},
        {8,0,0,0,0,0,0}
    };
*/
void createParseTable(){
    
    int row = 9;
    int col = 7;
    parseTable = malloc(sizeof(int*)*row);
    for (int i = 0; i < row; i++){
        parseTable[i] = malloc(sizeof(int)*col);
    }
    for (int i = 0; i < col; i++){
        parseTable[3][i] = 2;
        parseTable[4][i] = 4;
        parseTable[7][i] = 6;
    }
    parseTable[1][0] = 1;
    parseTable[2][0] = 3;
    parseTable[5][0] = 5;
    parseTable[6][0] = 7;
    parseTable[6][5] = 7;
    parseTable[8][0] = 8;
    
}

//choose rule to execute
Tree chooseRule(Tree curr,int rule){
    switch (rule)
    {
    case 1:
        return rule1(curr);
    case 2: 
        return rule2(curr);
    case 3:
        return rule3(curr);
    case 4:
        return rule4(curr);
    case 5:
        return rule5(curr);
    case 6:
        return rule6(curr);
    case 7:
        return rule7(curr);
    case 8:
        return rule8(curr);
    default:
        return ruleNegative(curr);
    }
}

//get rule from the table
int getRule(int row, int col){
    return parseTable[row][col];
}

//get Row based on label of Node
int getRow(char* label){
    char label1 = label[0];
    switch (label1){
        case 'E':
            if (label[1] == 'T'){
                return 3;
            }
            else if (label[1] == '\0'){
                return 1;
            }
            else return 0;
        case 'C':
            if (label[1] == 'T'){
                return 4;
            }
            else if (label[1] == '\0'){
                return 2;
            }
            else return 0; 
        case 'S':
            if (label[1] == 'T'){
                return 7;
            }
            else if (label[1] == '\0'){
                return 5;
            }
            else return 0; 
        case 'A':
            if (label[1] == '\0') return 6;
            else return 0;
        case 'X': 
            if (label[1] == '\0') return 8;
            else return 0;
        default:
            return 0;
    }
}

//choose Col based on input
int switchChar(char c){
    if ((int) c > 96 && (int) c < 123) return 0;
    else if (c == '.') return 1;
    else if (c == '*') return 2;
    else if (c == '|') return 3;
    else if (c == '\0') return 4;
    else if (c == '(') return 5;
    else if (c == ')') return 6;
    else return -1;
}


//pop operation of stack
Tree pop() {
    Tree c;
    if (size > 0){
        c = stack[size];
        size--;
    }
    return c;
}
//push operation of stack
void push(Tree c){
    if (size < MAX){
        size++;
        stack[size] = c;
    }
}

//rule for failed node
Tree ruleNegative(Tree curr) {
    push(FAILED);
    return FAILED;
}

// Rule 1: <E> -> <C><ET>
Tree rule1(Tree curr){
    int num = curr->indent+1;
    Tree concat = makeNode0("C",num);
    Tree et = makeNode0("ET",num);
    push(et);
    push(concat);
    return makeNode2("E",concat,et,num-1);
}

// Rule 3: <C> -> <S><CT>
Tree rule3(Tree curr){
    int num = curr->indent+1;
    Tree star = makeNode0("S",num);
    Tree ct = makeNode0("CT",num);
    push(ct);
    push(star);
    return makeNode2("C",star,ct,num-1);
}

// Rule 2: <ET> -> |<E>  |  eps
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

// Rule 5: <S> -> <A><ST>
Tree rule5(Tree curr){
    int num = curr->indent+1;
    Tree atomic, st;
    atomic = makeNode0("A",num);
    st = makeNode0("ST",num);
    push(st);
    push(atomic);
    return makeNode2("S",atomic,st,num-1);
}

// Rule 4: <CT> -> .<C>  |  eps
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

// Rule 7: <A> -> (<E>)  |  <X>
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
        push(e);
        return makeNode1("A",e,num-1);
    }
}

// Rule 6: <ST> -> *<ST> |  eps
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

// Rule 8: <X> -> a|b|c|...|z
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

//get printing line for node
void getLabel(char *x, int indent) {
    int index = 0;
    while (index < indent){
        printing[indexPrinting] = ' ';
        indexPrinting++;
        printing[indexPrinting] = ' ';
        indexPrinting++;
        index++;
    }
    int i = 0;
    while (x[i] != '\0'){
        printing[indexPrinting] = x[i];
        indexPrinting++;
        i++;
    }
    printing[indexPrinting] = '\n';
    indexPrinting++;
}

//print parse tree 
void printParseTree(){
    printing[indexPrinting] = '\0';
    int i = 0;
    while (printing[i] != '\0'){
        printf("%c",printing[i]);
        i++;
    }
    indexPrinting = 0;
}

void freeTable(int** parseTable){
    for (int i = 0; i < 9; i++){
        free(parseTable[i]);
    }
    free(parseTable);
}
