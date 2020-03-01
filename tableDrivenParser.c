#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tableDrivenParser.h"

#define MAX 1000
#define FAILED NULL

//TODO: memory leak



char printing[MAX];
int indexPrinting = 0;
char *start, *nextTerminal;
Tree stack[MAX];
int size = 0;
int** parseTable;

void createParseTable();
bool parsing();
int switchChar(char c);
int getRow(char* c);
int getRule(int row, int col);
void chooseRule(Tree curr, int rule);

void ruleNegative(Tree curr);
void rule1(Tree curr);
void rule2(Tree curr);
void rule3(Tree curr);
void rule4(Tree curr);
void rule5(Tree curr);
void rule6(Tree curr);
void rule7(Tree curr);
void rule8(Tree curr);

void freeTable(int **parseTable);

void tryTableDrivenParser(){
    start = (char*) malloc(sizeof(char)*256);
    createParseTable();
    bool flag = true;
    printf("-------------------------\n");
    printf("Trying Table Driven Parser...\n");
    while (flag){
        printf("\tEnter expression here (\"quit\" to quit and no more than 255 characters):");
        char input[256];
        scanf("%255s",input);
        if (strcmp(input,"quit") == 0){
            flag = false;
        }
        else {
            printf("Result for \"%s\":\n \n", input);
            nextTerminal = start;
            strcpy(nextTerminal,input);
            if (!parsing()){
                printError();
            }
            else {
                printParseTree();
            }
            while (size != 0){
                Tree curr = pop();
                freeTree(curr);
            }
        }
        printf("\n");
    }
    free(start);
    freeTable(parseTable);
    //freeStack();
    //free(printing);
}

//return if parsing is success
bool parsing(){
    Tree root = makeNode0("E",0);
    push(root);
    while (size != 0){
        //pop curr Node
        Tree curr = pop();
        if (curr == FAILED) return false;
        //check valid input and get column
        int col = switchChar(*nextTerminal);
        if (col == -1) {
            if (curr != FAILED) freeTree(curr);
            return false;
        }
        //get row by label
        int row = getRow(curr->label);
        int rule = getRule(row,col);
        if (row > 0){ //Capital Letter => label
            //look at position (row,col) => rule number
            //follow the rule
            chooseRule(curr,rule);
        }
        else { //matching (terminal node)
            if (strcmp(curr->label, "eps") != 0 && *nextTerminal != curr->label[0]){
                if (curr != FAILED) freeTree(curr);
                return false;
            } 
            else if (*nextTerminal == curr->label[0]) nextTerminal++;
        }
        if (curr != FAILED) {
            getLabel(curr->label,curr->indent);
            freeTree(curr);
        }
        else {
            return false;
        }
    }
    if (*nextTerminal != '\0') return false;
    return true;
}

/* 
    parseTable = {
        {0,0,0,0,0,0,0},
        {1,0,0,0,0,1,0},
        {3,0,0,0,0,3,0},
        {2,2,2,2,2,2,2},
        {4,4,4,4,4,4,4},
        {5,0,0,0,0,5,0},
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
        for (int j = 0; j < col; j++){
            parseTable[i][j] = 0;
        }
    }
    for (int i = 0; i < col; i++){
        parseTable[3][i] = 2;
        parseTable[4][i] = 4;
        parseTable[7][i] = 6;
    }
    parseTable[1][0] = 1;
    parseTable[1][5] = 1;
    parseTable[2][0] = 3;
    parseTable[2][5] = 3;
    parseTable[5][0] = 5;
    parseTable[5][5] = 5;
    parseTable[6][0] = 7;
    parseTable[6][5] = 7;
    parseTable[8][0] = 8;
    
}

//choose rule to execute
void chooseRule(Tree curr,int rule){
    switch (rule)
    {
    case 1:
        rule1(curr);
        break;
    case 2: 
        rule2(curr);
        break;
    case 3:
        rule3(curr);
        break;
    case 4:
        rule4(curr);
        break;
    case 5:
        rule5(curr);
        break;
    case 6:
        rule6(curr);
        break;
    case 7:
        rule7(curr);
        break;
    case 8:
        rule8(curr);
        break;
    default:
        ruleNegative(curr);
        break;
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
        size--;
        c = stack[size];
    }
    return c;
}
//push operation of stack
void push(Tree c){
    if (size < MAX){
        stack[size] = c;
        size++;
    }
}

//rule for failed node
void ruleNegative(Tree curr) {
    push(FAILED);
    return;
}

// Rule 1: <E> -> <C><ET>
void rule1(Tree curr){
    int num = curr->indent+1;
    Tree concat = makeNode0("C",num);
    Tree et = makeNode0("ET",num);
    push(et);
    push(concat);
    return;
}

// Rule 3: <C> -> <S><CT>
void rule3(Tree curr){
    int num = curr->indent+1;
    Tree star = makeNode0("S",num);
    Tree ct = makeNode0("CT",num);
    push(ct);
    push(star);
}

// Rule 2: <ET> -> |<E>  |  eps
void rule2(Tree curr){
    int num = curr->indent+1;
    if (*nextTerminal == '|'){
        Tree e = makeNode0("E",num);
        Tree straight = makeNode0("|",num);
        push(e);
        push(straight);
        return;
    }
    else {
        Tree eps = makeNode0("eps",num);
        push(eps);
        return;
    }
}

// Rule 5: <S> -> <A><ST>
void rule5(Tree curr){
    int num = curr->indent+1;
    Tree atomic, st;
    atomic = makeNode0("A",num);
    st = makeNode0("ST",num);
    push(st);
    push(atomic);
    return;
}

// Rule 4: <CT> -> .<C>  |  eps
void rule4(Tree curr){
    int num = curr->indent+1;
    if (*nextTerminal == '.'){
        Tree c;
        Tree dot = makeNode0(".",num);
        c = makeNode0("C",num);
        push(c);
        push(dot);
        return;
    }
    else{
        Tree eps = makeNode0("eps",num);
        push(eps);
        return;
    }
}

// Rule 7: <A> -> (<E>)  |  <X>
void rule7(Tree curr){
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
        return;
    }
    else {
        Tree e;
        e = makeNode0("X",num);
        push(e);
        return;
    }
}

// Rule 6: <ST> -> *<ST> |  eps
void rule6(Tree curr){
    int num = curr->indent+1;
    if (*nextTerminal == '*'){
        Tree st;
        Tree star;
        st = makeNode0("ST",num);
        star = makeNode0("*",num);
        push(st);
        push(star);
        return;
    }
    else {
        Tree eps = makeNode0("eps",num);
        push(eps);
        return;
    }
}

// Rule 8: <X> -> a|b|c|...|z
void rule8(Tree curr){
    char c = *nextTerminal;
    int num = curr->indent+1;
    if ((int) c > 96 && (int) c < 123){
        //nextTerminal++;
        char *input = malloc(2*sizeof(char));
        input[0] = c;
        input[1] = '\0';
        Tree in = makeNode0(input,num);
        in->freeable = true;
        push(in);
        return;
    }
    else {
        Tree c = FAILED;
        push(c);
        return;
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
        //printf("  ");
    }
    int i = 0;
    while (x[i] != '\0'){
        printing[indexPrinting] = x[i];
        indexPrinting++;
        i++;
    }
    //printf("%s\n",x);
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
