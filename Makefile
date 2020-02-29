CFLAGS = -g -std=c99 -Wall -Werror

rexp: main.o Tree.o convertToExpressionTree.o recursiveDescentParser.o tableDrivenParser.o
	$(CC) -o $@ $(CFLAGS) $^

valgrind:
	make clean && make rexp && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./rexp

test: main.o Tree.o recursiveDescentParser.o tableDrivenParser.o
	$(CC) -o $@ $(CFLAGS) $^

clean:
	-rm *.o test rexp valgrind-out.txt
