#ifndef _Recursive_h
#define _Recursive_h

#include "Tree.h"

#define FAILED NULL

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

#endif