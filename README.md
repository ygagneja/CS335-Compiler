# CS335-Compiler
 C to x86 compiler developed under CS335 at IIT Kanpur
 
 ### Steps to build and run lexer :
 ```ruby
 make
 ./bin/lexer -help  #To see the Usage Info
 ./bin/lexer -i [path to the test program] -o [Output file (default is STDOUT)]
 ```
 ### Example test commands:
 ```ruby
 ./bin/lexer -i ./tests/test1.c
 ./bin/lexer -i ./tests/test1.c -o result.txt
 ```
 ### To remove all the binaries and intermediate files :
```ruby
make clean
```
