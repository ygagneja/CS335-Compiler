CC=gcc
LEX=flex
.PHONY: all clean

all: lexer

lexer : lex.yy.c parse.tab.h bin
	$(CC) -o ../bin/lexer lex.yy.c

bin:
	mkdir -p ../bin

lex.yy.c : lexer.l
	$(LEX) lexer.l

clean:
	rm lex.yy.c
	rm -rf ../bin/*
