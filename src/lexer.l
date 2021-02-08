D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%{
#include <stdio.h>
#include <stdlib.h>
#include "parse.tab.h"

int yywrap();
void comment();
void count(char* token);

int column = 1;
int line = 1;
%}

%%
"/*"			{ comment(); }
"//".*          {}

"auto"			{ count("AUTO"); return(AUTO); }
"break"			{ count("BREAK"); return(BREAK); }
"case"			{ count("CASE"); return(CASE); }
"char"			{ count("CHAR"); return(CHAR); }
"const"			{ count("CONST"); return(CONST); }
"continue"		{ count("CONTINUE"); return(CONTINUE); }
"default"		{ count("DEFAULT"); return(DEFAULT); }
"do"			{ count("DO"); return(DO); }
"double"		{ count("DOUBLE"); return(DOUBLE); }
"else"			{ count("ELSE"); return(ELSE); }
"enum"			{ count("ENUM"); return(ENUM); }
"extern"		{ count("EXTERN"); return(EXTERN); }
"float"			{ count("FLOAT"); return(FLOAT); }
"for"			{ count("FOR"); return(FOR); }
"goto"			{ count("GOTO"); return(GOTO); }
"if"			{ count("IF"); return(IF); }
"int"			{ count("INT"); return(INT); }
"long"			{ count("LONG"); return(LONG); }
"register"		{ count("REGISTER"); return(REGISTER); }
"return"		{ count("RETURN"); return(RETURN); }
"short"			{ count("SHORT"); return(SHORT); }
"signed"		{ count("SIGNED"); return(SIGNED); }
"sizeof"		{ count("SIZEOF"); return(SIZEOF); }
"static"		{ count("STATIC"); return(STATIC); }
"struct"		{ count("STRUCT"); return(STRUCT); }
"switch"		{ count("SWITCH"); return(SWITCH); }
"typedef"		{ count("TYPEDEF"); return(TYPEDEF); }
"union"			{ count("UNION"); return(UNION); }
"unsigned"		{ count("UNSIGNED"); return(UNSIGNED); }
"void"			{ count("VOID"); return(VOID); }
"volatile"		{ count("VOLATILE"); return(VOLATILE); }
"while"			{ count("WHILE"); return(WHILE); }

{L}({L}|{D})*		{ count("IDENTIFIER"); return(IDENTIFIER); }

0[xX]{H}+{IS}?		{ count("CONSTANT"); return(CONSTANT); }
0{D}+{IS}?		{ count("CONSTANT"); return(CONSTANT); }
{D}+{IS}?		{ count("CONSTANT"); return(CONSTANT); }
L?'(\\.|[^\\'])+'	{ count("CONSTANT"); return(CONSTANT); }

{D}+{E}{FS}?		{ count("CONSTANT"); return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?	{ count("CONSTANT"); return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?	{ count("CONSTANT"); return(CONSTANT); }

L?\"(\\.|[^\\"])*\"	{ count("STRING_LITERAL"); return(STRING_LITERAL); }

"..."			{ count("ELLIPSIS"); return(ELLIPSIS); }
">>="			{ count("RIGHT_ASSIGN"); return(RIGHT_ASSIGN); }
"<<="			{ count("LEFT_ASSIGN"); return(LEFT_ASSIGN); }
"+="			{ count("ADD_ASSIGN"); return(ADD_ASSIGN); }
"-="			{ count("SUB_ASSIGN"); return(SUB_ASSIGN); }
"*="			{ count("MUL_ASSIGN"); return(MUL_ASSIGN); }
"/="			{ count("DIV_ASSIGN"); return(DIV_ASSIGN); }
"%="			{ count("MOD_ASSIGN"); return(MOD_ASSIGN); }
"&="			{ count("AND_ASSIGN"); return(AND_ASSIGN); }
"^="			{ count("XOR_ASSIGN"); return(XOR_ASSIGN); }
"|="			{ count("OR_ASSIGN"); return(OR_ASSIGN); }
">>"			{ count("RIGHT_OP"); return(RIGHT_OP); }
"<<"			{ count("LEFT_OP"); return(LEFT_OP); }
"++"			{ count("INC_OP"); return(INC_OP); }
"--"			{ count("DEC_OP"); return(DEC_OP); }
"->"			{ count("PTR_OP"); return(PTR_OP); }
"&&"			{ count("AND_OP"); return(AND_OP); }
"||"			{ count("OR_OP"); return(OR_OP); }
"<="			{ count("LE_OP"); return(LE_OP); }
">="			{ count("GE_OP"); return(GE_OP); }
"=="			{ count("EQ_OP"); return(EQ_OP); }
"!="			{ count("NE_OP"); return(NE_OP); }
";"			{ count(";"); return(';'); }
("{"|"<%")		{ count("{"); return('{'); }
("}"|"%>")		{ count("}"); return('}'); }
","			{ count(","); return(','); }
":"			{ count(":"); return(':'); }
"="			{ count("="); return('='); }
"("			{ count("("); return('('); }
")"			{ count(")"); return(')'); }
("["|"<:")		{ count("["); return('['); }
("]"|":>")		{ count("]"); return(']'); }
"."			{ count("."); return('.'); }
"&"			{ count("&"); return('&'); }
"!"			{ count("!"); return('!'); }
"~"			{ count("~"); return('~'); }
"-"			{ count("-"); return('-'); }
"+"			{ count("+"); return('+'); }
"*"			{ count("*"); return('*'); }
"/"			{ count("/"); return('/'); }
"%"			{ count("%"); return('%'); }
"<"			{ count("<"); return('<'); }
">"			{ count(">"); return('>'); }
"^"			{ count("^"); return('^'); }
"|"			{ count("|"); return('|'); }
"?"			{ count("?"); return('?'); }

[ \t\v\n\f]		{ count("\0"); }
.			{ fprintf (stderr, "\n\nUnidentified expression found in line %d, terminating...\n\n", line); return 0; }

%%

int yywrap(){
    return 1;
}

void comment(){
    int c;
    while ((c = input()) != 0){
        if (c=='\n'){
            column = 1;
            line++;
        }
        // else if (c=='\t') column = column + 8 - (column%8);
        else column++;

        if (c == '*'){
            while ((c = input()) == '*') column++;
            if (c) column++;
            if (c == '/' || c == 0) return;
        }
    }
    return;
}

void count(char* token){
    int prev_col = column;
    for (int i=0; yytext[i]!='\0'; i++){
        if (yytext[i]=='\n'){
            column = 1;
            line++;
        }
        // else if (yytext[i]=='\t'){
        //     column = column + 8 - (column%8);
        // }
        else column++;
    }
    if (token[0]!='\0'){
        printf ("%s\t", token);
        if (strlen(token) < 8) printf ("\t");
        printf ("%s\t", yytext);
        if (strlen(yytext) < 8) printf ("\t");
        printf ("%d\t\t%d\n", line, prev_col);
    }
}

int main (int argc, char* argv[]){
    yyin = fopen (argv[1], "r");
    printf ("Token\t\tLexeme\t\tLine#\t\tColumn#\n");
    while (yylex() != 0);
    fclose (yyin);
    return 0;
}