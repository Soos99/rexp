#ifndef _Table_h
#define _Table_h

#include "Tree.h"

void getLabel(char *x, int indent);
void printParseTree();
void printError();

bool parsing();

Tree pop();
void push(Tree curr);


void tryTableDrivenParser();

#endif