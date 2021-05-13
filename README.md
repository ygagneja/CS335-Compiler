# CS335-Compiler
 C to MIPS compiler developed under CS335 at IIT Kanpur
 
 ### Steps to build and run the compiler :
 ```ruby
 make
 ./bin/compiler -help  # To see the usage info
 ./bin/compiler -i <path to the test file> [-link]
 # "-link" option will link a few math and string library functions with out.asm
 # All the symbol tables will be dumped inside "/out/sym_tables"
 # All the type tables will be dumped inside "/out/type_tables"
 # 3AC will be dumped in "out/out.3ac"
 # ASM code will be dumped in "/out.asm"
 # AST will be dumped in "out/tree.ast"
 
 # To generate the image of the AST
 dot -Tps tree.ast -o graph.ps
 
 # To run the assembly code
 sudo apt-get install spim # Install spim simulator if not present
 spim # To start the spim simulator
 # To simulate the generated MIPS assembly, run following commands on the spim simulator
 load "out.asm"
 run
 ```
 ### Example test commands:
 ```ruby
 ./bin/compiler -i ./tests/test1.c
 dot -Tps tree.ast -o graph.ps # Open file "graph.ps" to see the generated AST
 spim
 load "out.asm"
 run
 ```
 ### To remove all the binaries and intermediate files :
```ruby
make clean
```

### Project Specifications ###

* Source Language : C
* Implementation Language : C++
* Target Language : MIPS

### Team Members ###
Group-16
* Avinandan Bose (180165)
* Spandan Senapati (180782)
* Yuvraj (180898)
* Yuvraj Garg (180900)