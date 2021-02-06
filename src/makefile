CC=gcc
LEX=flex
.PHONY: all clean

all: lexer

lexer : lex.yy.c parse.tab.h
	$(CC) -o ../bin/lexer lex.yy.c

lex.yy.c : lexer.l
	$(LEX) lexer.l

clean:
	rm lex.yy.c
	rm -rf ../bin/*
