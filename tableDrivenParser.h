#ifndef _Table_h
#define _Table_h

#include "Tree.h"

void getLabel(char *x, int indent);
void printParseTree();
void printError();

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

void tryTableDrivenParser();

#endif