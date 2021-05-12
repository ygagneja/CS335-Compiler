CC=g++ -std=c++17 -w -g
LEX=flex
YACC=bison
.PHONY: all clean

all: ./bin/compiler

./bin/compiler : ./src/lex.yy.c ./src/parser.tab.c ./src/nodes.* ./src/type_check.* ./src/sym_table.* ./src/3ac.* ./src/codegen.*
	mkdir -p bin
	mkdir -p out
	mkdir -p ./out/sym_tables
	mkdir -p ./out/type_tables
	$(CC) $^ -o $@ -I ./src

./src/parser.tab.c : ./src/parser.y
	$(YACC) -d $^ -o $@

./src/lex.yy.c : ./src/lexer.l
	$(LEX) -o $@ $^

clean:
	rm -rf ./src/lex.yy.c ./src/parser.tab.* ./src/*.o
	rm -rf bin
	rm -rf out
	rm -rf *.ast *.ps *.csv *.asm *.3ac