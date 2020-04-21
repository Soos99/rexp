# Rexp
Different implementations of a parser for a context-free grammar of regular expressions (recursive-descent and table-driven) as well as finite automaton transformation. 

## Grammar Structure
1. <E> -> <C><ET>
2. <ET> -> |E  |  eps
3. <C> -> <S><CT>
4. <CT> -> .<C> | eps
5. <S> -> <A><ST>
6. <ST> -> *<ST> | eps
7. <A> -> (<E>) | <X>
8. <X> -> a|b|...|z

## Features 
1. Recursive-descent Parser to generate Parse Tree
2. Table-driven Parser to generate Parse Tree
3. Finite Automaton Transformation from Parse Tree
4. Memory leak checking using [Valgrind](http://valgrind.org/)

## Running instructions
1. Install [Make](https://www.gnu.org/software/make/) and [Valgrind](http://valgrind.org/)
2. Run program (Memory leak checking report will be automatically generated)
```
$ make rexp
$ ./rexp
```
3. Clean object files
```
$ make clean
```
## Contributors
1. [Hoang Le](https://github.com/Soos99)
2. [Loc Bui](https://github.com/hekl0)



