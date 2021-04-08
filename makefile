CC=g++ -ggdb3 -std=c++11 -w
LEX=flex
YACC=bison
.PHONY: all clean

all: ./bin/parser

./bin/parser : ./src/lex.yy.c ./src/parser.tab.c ./src/nodes.* ./src/type_check.* ./src/sym_table.*
	mkdir -p bin
	$(CC) -g $^ -o $@ -I ./src

./src/parser.tab.c : ./src/parser.y
	$(YACC) -d $^ -o $@

./src/lex.yy.c : ./src/lexer.l
	$(LEX) -o $@ $^

# ./src/nodes.o : ./src/nodes.cpp ./src/nodes.h
# 	$(CC) -c $^ -o $@

# ./src/type_check.o : ./src/type_check.cpp ./src/type_check.h
# 	$(CC) -c $^ -o $@

# ./src/sym_table.o : ./src/sym_table.cpp ./src/sym_table.h
# 	$(CC) -c $^ -o $@

clean:
	rm -rf ./src/lex.yy.c ./src/parser.tab.* ./src/*.o
	rm -rf bin
	rm -rf *.dot
	rm -rf *.ps
	rm -rf *.csv