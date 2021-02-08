CC=gcc
LEX=flex
.PHONY: all clean

all: ./bin/lexer

./bin/lexer : ./src/lex.yy.c ./src/parse.tab.h bin
	$(CC) -o ./bin/lexer ./src/lex.yy.c

bin:
	mkdir -p bin

./src/lex.yy.c : ./src/lexer.l
	$(LEX) -o ./src/lex.yy.c ./src/lexer.l

clean:
	rm ./src/lex.yy.c
	rm -rf bin
