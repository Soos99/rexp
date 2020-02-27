#include <stdlib.h>
#include <stdio.h>
#include "convertToExpressionTree.h"
#include "recursiveDescentParser.h"

void tryConvert() {
    Tree parseTree;
    char *input = "a.b.c";

    parseTree = buildRecursiveDescentParse(input);
    printTree(parseTree, 0);
} 

Tree convertToExpressionTree(Tree root) {
    return NULL;
}
void printExpressionTree(Tree root) {

}