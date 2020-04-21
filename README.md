# Rexp
Different implementations of a parser for a context-free grammar of regular expressions (recursive-descent and table-driven) as well as finite automaton transformation. 

## Grammar Structure
1. &lt;E&gt; -&gt; &lt;C&gt;&lt;ET&gt;
2. &lt;ET&gt; -&gt; |E  |  eps
3. &lt;C&gt; -&gt; &lt;S&gt;&lt;CT&gt;
4. &lt;CT&gt; -&gt; .&lt;C&gt; | eps
5. &lt;S&gt; -&gt; &lt;A&gt;&lt;ST&gt;
6. &lt;ST&gt; -&gt; *&lt;ST&gt; | eps
7. &lt;A&gt; -&gt; (&lt;E&gt;) | &lt;X&gt;
8. &lt;X&gt; -&gt; a|b|...|z

__*Note:__ "eps" means empty character

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



