# CS335-Compiler
 C to x86 compiler developed under CS335 at IIT Kanpur
 
 ### Steps to build and run parser (semantics taken care of) :
 ```ruby
 make
 ./bin/parser -help  #To see the Usage Info
 ./bin/parser -i [path to the test program] -o [Output AST file] #This will dumpthe dot file (for AST) and the symbol tables in csv format if the test code is semantically correct
 dot -Tps graph1.dot -o graph1.ps #To generate the image of the Abstract Syntax Tree
 ```
 ### Example test commands:
 ```ruby
 ./bin/parser -i ./tests/test6.c -o myAST.dot
 dot -Tps myAST.dot -o graph1.ps
 #Open file "graph1.ps" to see the generated AST
 ```
 ### To remove all the binaries and intermediate files :
```ruby
make clean
```
