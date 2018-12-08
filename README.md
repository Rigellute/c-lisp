### Goals
1. Learn more about the C programming language.
1. Learn about language design
1. Learn about compilers

This is based on the [`buildyourownlisp`](http://www.buildyourownlisp.com/)

Current Chapter: [9: S-Expressions](http://www.buildyourownlisp.com/chapter9_s_expressionshttp://www.buildyourownlisp.com/chapter9_s_expressions)

##### Compilation
On Mac
```sh
cc -std=c99 -Wall parsing.c mpc.c -ledit -lm -o parsing
```

On Windows
```sh
gcc -std=c99 -Wall parsing.c mpc.c -o parsing
```