D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "nodes.h"
#include "sym_table.h"
#include "parser.tab.h"

using namespace std;

void comment();
void count(char* token);
void yyerror(char* s);
int is_type();

int column = 1;
int line = 1;
%}

%%
"/*"			{ comment(); }
"//".*          {}

"auto"			{ yylval.str=strdup(yytext); count("AUTO"); return(AUTO); }
"break"			{ yylval.str=strdup(yytext); count("BREAK"); return(BREAK); }
"case"			{ yylval.str=strdup(yytext); count("CASE"); return(CASE); }
"char"			{ yylval.str=strdup(yytext); count("CHAR"); return(CHAR); }
"const"			{ yylval.str=strdup(yytext); count("CONST"); return(CONST); }
"continue"		{ yylval.str=strdup(yytext); count("CONTINUE"); return(CONTINUE); }
"default"		{ yylval.str=strdup(yytext); count("DEFAULT"); return(DEFAULT); }
"do"			{ yylval.str=strdup(yytext); count("DO"); return(DO); }
"double"		{ yylval.str=strdup(yytext); count("DOUBLE"); return(DOUBLE); }
"else"			{ yylval.str=strdup(yytext); count("ELSE"); return(ELSE); }
"enum"			{ yylval.str=strdup(yytext); count("ENUM"); return(ENUM); }
"extern"		{ yylval.str=strdup(yytext); count("EXTERN"); return(EXTERN); }
"float"			{ yylval.str=strdup(yytext); count("FLOAT"); return(FLOAT); }
"for"			{ yylval.str=strdup(yytext); count("FOR"); return(FOR); }
"goto"			{ yylval.str=strdup(yytext); count("GOTO"); return(GOTO); }
"if"			{ yylval.str=strdup(yytext); count("IF"); return(IF); }
"int"			{ yylval.str=strdup(yytext); count("INT"); return(INT); }
"long"			{ yylval.str=strdup(yytext); count("LONG"); return(LONG); }
"register"		{ yylval.str=strdup(yytext); count("REGISTER"); return(REGISTER); }
"return"		{ yylval.str=strdup(yytext); count("RETURN"); return(RETURN); }
"short"			{ yylval.str=strdup(yytext); count("SHORT"); return(SHORT); }
"signed"		{ yylval.str=strdup(yytext); count("SIGNED"); return(SIGNED); }
"sizeof"		{ yylval.str=strdup(yytext); count("SIZEOF"); return(SIZEOF); }
"static"		{ yylval.str=strdup(yytext); count("STATIC"); return(STATIC); }
"struct"		{ yylval.str=strdup(yytext); count("STRUCT"); return(STRUCT); }
"switch"		{ yylval.str=strdup(yytext); count("SWITCH"); return(SWITCH); }
"typedef"		{ yylval.str=strdup(yytext); count("TYPEDEF"); return(TYPEDEF); }
"union"			{ yylval.str=strdup(yytext); count("UNION"); return(UNION); }
"unsigned"		{ yylval.str=strdup(yytext); count("UNSIGNED"); return(UNSIGNED); }
"void"			{ yylval.str=strdup(yytext); count("VOID"); return(VOID); }
"volatile"		{ yylval.str=strdup(yytext); count("VOLATILE"); return(VOLATILE); }
"while"			{ yylval.str=strdup(yytext); count("WHILE"); return(WHILE); }

{L}({L}|{D})*		{ return is_type(); }

0[xX]{H}+{IS}?		{ yylval.str=strdup(yytext); count("CONSTANT"); return(CONSTANT); }
0{D}+{IS}?		{ yylval.str=strdup(yytext); count("CONSTANT"); return(CONSTANT); }
{D}+{IS}?		{ yylval.str=strdup(yytext); count("CONSTANT"); return(CONSTANT); }
L?'(\\.|[^\\'])+'	{ yylval.str=strdup(yytext); count("CONSTANT"); return(CONSTANT); }

{D}+{E}{FS}?		{ yylval.str=strdup(yytext); count("CONSTANT"); return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?	{ yylval.str=strdup(yytext); count("CONSTANT"); return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?	{ yylval.str=strdup(yytext); count("CONSTANT"); return(CONSTANT); }

L?\"(\\.|[^\\"])*\"	{ yylval.str=strdup(yytext); count("STRING_LITERAL"); return(STRING_LITERAL); }

"..."			{ yylval.str=strdup(yytext); count("ELLIPSIS"); return(ELLIPSIS); }
">>="			{ yylval.str=strdup(yytext); count("RIGHT_ASSIGN"); return(RIGHT_ASSIGN); }
"<<="			{ yylval.str=strdup(yytext); count("LEFT_ASSIGN"); return(LEFT_ASSIGN); }
"+="			{ yylval.str=strdup(yytext); count("ADD_ASSIGN"); return(ADD_ASSIGN); }
"-="			{ yylval.str=strdup(yytext); count("SUB_ASSIGN"); return(SUB_ASSIGN); }
"*="			{ yylval.str=strdup(yytext); count("MUL_ASSIGN"); return(MUL_ASSIGN); }
"/="			{ yylval.str=strdup(yytext); count("DIV_ASSIGN"); return(DIV_ASSIGN); }
"%="			{ yylval.str=strdup(yytext); count("MOD_ASSIGN"); return(MOD_ASSIGN); }
"&="			{ yylval.str=strdup(yytext); count("AND_ASSIGN"); return(AND_ASSIGN); }
"^="			{ yylval.str=strdup(yytext); count("XOR_ASSIGN"); return(XOR_ASSIGN); }
"|="			{ yylval.str=strdup(yytext); count("OR_ASSIGN"); return(OR_ASSIGN); }
">>"			{ yylval.str=strdup(yytext); count("RIGHT_OP"); return(RIGHT_OP); }
"<<"			{ yylval.str=strdup(yytext); count("LEFT_OP"); return(LEFT_OP); }
"++"			{ yylval.str=strdup(yytext); count("INC_OP"); return(INC_OP); }
"--"			{ yylval.str=strdup(yytext); count("DEC_OP"); return(DEC_OP); }
"->"			{ yylval.str=strdup(yytext); count("PTR_OP"); return(PTR_OP); }
"&&"			{ yylval.str=strdup(yytext); count("AND_OP"); return(AND_OP); }
"||"			{ yylval.str=strdup(yytext); count("OR_OP"); return(OR_OP); }
"<="			{ yylval.str=strdup(yytext); count("LE_OP"); return(LE_OP); }
">="			{ yylval.str=strdup(yytext); count("GE_OP"); return(GE_OP); }
"=="			{ yylval.str=strdup(yytext); count("EQ_OP"); return(EQ_OP); }
"!="			{ yylval.str=strdup(yytext); count("NE_OP"); return(NE_OP); }
";"			{ yylval.str=strdup(yytext); count(";"); return(';'); }
("{"|"<%")		{ yylval.str=strdup(yytext); count("{"); return('{'); }
("}"|"%>")		{ yylval.str=strdup(yytext); count("}"); return('}'); }
","			{ yylval.str=strdup(yytext); count(","); return(','); }
":"			{ yylval.str=strdup(yytext); count(":"); return(':'); }
"="			{ yylval.str=strdup(yytext); count("="); return('='); }
"("			{ yylval.str=strdup(yytext); count("("); return('('); }
")"			{ yylval.str=strdup(yytext); count(")"); return(')'); }
("["|"<:")		{ yylval.str=strdup(yytext); count("["); return('['); }
("]"|":>")		{ yylval.str=strdup(yytext); count("]"); return(']'); }
"."			{ yylval.str=strdup(yytext); count("."); return('.'); }
"&"			{ yylval.str=strdup(yytext); count("&"); return('&'); }
"!"			{ yylval.str=strdup(yytext); count("!"); return('!'); }
"~"			{ yylval.str=strdup(yytext); count("~"); return('~'); }
"-"			{ yylval.str=strdup(yytext); count("-"); return('-'); }
"+"			{ yylval.str=strdup(yytext); count("+"); return('+'); }
"*"			{ yylval.str=strdup(yytext); count("*"); return('*'); }
"/"			{ yylval.str=strdup(yytext); count("/"); return('/'); }
"%"			{ yylval.str=strdup(yytext); count("%"); return('%'); }
"<"			{ yylval.str=strdup(yytext); count("<"); return('<'); }
">"			{ yylval.str=strdup(yytext); count(">"); return('>'); }
"^"			{ yylval.str=strdup(yytext); count("^"); return('^'); }
"|"			{ yylval.str=strdup(yytext); count("|"); return('|'); }
"?"			{ yylval.str=strdup(yytext); count("?"); return('?'); }

[ \t\v\n\f]		{ count("\0"); }
.			{ yyerror("Unidentified expression found !!!\n");}

%%

int yywrap(void) {
    return 1;
} 

void comment(){
    int c;
    while ((c = yyinput()) != 0){
        if (c=='\n'){
            column = 1;
            line++;
        }
        // else if (c=='\t') column = column + 8 - (column%8);
        else column++;

        if (c == '*'){
            while ((c = yyinput()) == '*') column++;
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
}

int is_type(){
    yylval.str=strdup(yytext);
    count("IDENTIFIER");
    if (lookup_type(string(yytext))){
        return TYPE_NAME;
    }
    else {
        return IDENTIFIER;
    }
}