# CS335-Compiler
 C to x86 compiler developed under CS335 at IIT Kanpur
 
 ### Steps to build and run parser :
 ```ruby
 make
 ./bin/parser -help  #To see the Usage Info
 ./bin/parser -i [path to the test program] -o [Output AST file]
 dot -Tps graph1.dot -o graph1.ps #To generate the image of the parse tree
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
