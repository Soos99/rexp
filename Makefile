CFLAGS = -g -std=c99 -Wall -Werror

rexp: main.o Tree.o convertToExpressionTree.o recursiveDescentParser.o tableDrivenParser.o
	$(CC) -o $@ $(CFLAGS) $^

test: main.o Tree.o recursiveDescentParser.o tableDrivenParser.o
	$(CC) -o $@ $(CFLAGS) $^

clean:
	-rm *.o test rexp