#include <stdlib.h>
#include <stdio.h>

#include "recursiveDescentParser.h"
#include "tableDrivenParser.h"
#include "convertToExpressionTree.h"

int main() {
    tryRecursiveDescentParser();
    tryTableDrivenParser();
    tryConvert();
    return 0;
}