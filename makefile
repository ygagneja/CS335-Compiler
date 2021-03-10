CC=g++ -ggdb3 -std=c++11 -w
LEX=flex
YACC=bison
.PHONY: all clean

all: ./bin/parser

./bin/parser : ./src/lex.yy.c ./src/parser.tab.c ./src/nodes.o
	mkdir -p bin
	$(CC) $^ -o $@

./src/parser.tab.c : ./src/parser.y
	$(YACC) -d $^ -o $@

./src/lex.yy.c : ./src/lexer.l
	$(LEX) -o $@ $^

./src/nodes.o : ./src/nodes.cpp
	$(CC) -c $^ -o $@

clean:
	rm -rf ./src/lex.yy.c ./src/parser.tab.* ./src/*.o
	rm -rf bin
	rm -rf *.dot
	rm -rf *.ps