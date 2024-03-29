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
#include "parser.tab.h"

#define MAX_LEVELS 1024
using namespace std;

void comment();
void count(char* token);
void yyerror(char* s);

extern unsigned long long level;
extern unsigned long long level_id[MAX_LEVELS];
int column = 1;
int line = 1;
%}

%%
"/*"			{ comment(); }
"//".*          {}

"auto"			{ yylval.str=strdup(yytext); count("AUTO"); return(AUTO); }
"bool"          { yylval.str=strdup(yytext); count("BOOL"); return(BOOL); }
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
"false"         { yylval.str=strdup(yytext); count("FALSE"); return(FALSE); }
"float"			{ yylval.str=strdup(yytext); count("FLOAT"); return(FLOAT); }
"for"			{ yylval.str=strdup(yytext); count("FOR"); return(FOR); }
"goto"			{ yylval.str=strdup(yytext); count("GOTO"); return(GOTO); }
"if"			{ yylval.str=strdup(yytext); count("IF"); return(IF); }
"int"			{ yylval.str=strdup(yytext); count("INT"); return(INT); }
"long"			{ yylval.str=strdup(yytext); count("LONG"); return(LONG); }
"NULL"          { yylval.constant->str=strdup(yytext); count("NULL"); return(NULL_PTR);}
"register"		{ yylval.str=strdup(yytext); count("REGISTER"); return(REGISTER); }
"return"		{ yylval.str=strdup(yytext); count("RETURN"); return(RETURN); }
"short"			{ yylval.str=strdup(yytext); count("SHORT"); return(SHORT); }
"signed"		{ yylval.str=strdup(yytext); count("SIGNED"); return(SIGNED); }
"sizeof"		{ yylval.str=strdup(yytext); count("SIZEOF"); return(SIZEOF); }
"static"		{ yylval.str=strdup(yytext); count("STATIC"); return(STATIC); }
"struct"		{ yylval.str=strdup(yytext); count("STRUCT"); return(STRUCT); }
"switch"		{ yylval.str=strdup(yytext); count("SWITCH"); return(SWITCH); }
"true"          { yylval.str=strdup(yytext); count("TRUE"); return(TRUE); }
"typedef"		{ yylval.str=strdup(yytext); count("TYPEDEF"); return(TYPEDEF); }
"union"			{ yylval.str=strdup(yytext); count("UNION"); return(UNION); }
"unsigned"		{ yylval.str=strdup(yytext); count("UNSIGNED"); return(UNSIGNED); }
"void"			{ yylval.str=strdup(yytext); count("VOID"); return(VOID); }
"volatile"		{ yylval.str=strdup(yytext); count("VOLATILE"); return(VOLATILE); }
"while"			{ yylval.str=strdup(yytext); count("WHILE"); return(WHILE); }

{L}({L}|{D})*		{ yylval.str=strdup(yytext); count("IDENTIFIER"); return(IDENTIFIER); }

0[xX]{H}+   		{ sscanf(yytext+2,"%x", &yylval.constant->int_val); yylval.constant->str = strdup(yytext); count("INT_C"); return(INT_C); }
0{D}+       		{ sscanf(yytext,"%d", &yylval.constant->int_val); yylval.constant->str = strdup(yytext); count("INT_C"); return(INT_C); }
{D}+        		{ sscanf(yytext,"%d", &yylval.constant->int_val); yylval.constant->str = strdup(yytext); count("INT_C"); return(INT_C); }

{D}+{E}{FS}?		{ sscanf (yytext, "%f", &yylval.constant->float_val); yylval.constant->str = strdup(yytext); count("FLOAT_C"); return(FLOAT_C); }
{D}*"."{D}+({E})?	{ sscanf (yytext, "%f", &yylval.constant->float_val); yylval.constant->str = strdup(yytext); count("FLOAT_C"); return(FLOAT_C); }
{D}+"."{D}*({E})?	{ sscanf (yytext, "%f", &yylval.constant->float_val); yylval.constant->str = strdup(yytext); count("FLOAT_C"); return(FLOAT_C); }

L?\"(\\.|[^\\"])*\"	{ yylval.str=strdup(yytext); count("STRING_LITERAL"); return(STRING_LITERAL); }
\'.\'               { sscanf (yytext+1, "%c", &yylval.constant->int_val); yylval.constant->str=strdup(yytext); count("CHAR_LITERAL"); return(CHAR_LITERAL); }

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
";"			    { yylval.str=strdup(yytext); count(";"); return(';'); }
("{"|"<%")		{ yylval.str=strdup(yytext); count("{"); return('{'); }
("}"|"%>")		{ yylval.str=strdup(yytext); count("}"); return('}'); }
","			    { yylval.str=strdup(yytext); count(","); return(','); }
":"			    { yylval.str=strdup(yytext); count(":"); return(':'); }
"="			    { yylval.str=strdup(yytext); count("="); return('='); }
"("			    { yylval.str=strdup(yytext); count("("); return('('); }
")"			    { yylval.str=strdup(yytext); count(")"); return(')'); }
("["|"<:")		{ yylval.str=strdup(yytext); count("["); return('['); }
("]"|":>")		{ yylval.str=strdup(yytext); count("]"); return(']'); }
"."			    { yylval.str=strdup(yytext); count("."); return('.'); }
"&"			    { yylval.str=strdup(yytext); count("&"); return('&'); }
"!"			    { yylval.str=strdup(yytext); count("!"); return('!'); }
"~"			    { yylval.str=strdup(yytext); count("~"); return('~'); }
"-"			    { yylval.str=strdup(yytext); count("-"); return('-'); }
"+"			    { yylval.str=strdup(yytext); count("+"); return('+'); }
"*"			    { yylval.str=strdup(yytext); count("*"); return('*'); }
"/"			    { yylval.str=strdup(yytext); count("/"); return('/'); }
"%"			    { yylval.str=strdup(yytext); count("%"); return('%'); }
"<"			    { yylval.str=strdup(yytext); count("<"); return('<'); }
">"			    { yylval.str=strdup(yytext); count(">"); return('>'); }
"^"			    { yylval.str=strdup(yytext); count("^"); return('^'); }
"|"			    { yylval.str=strdup(yytext); count("|"); return('|'); }
"?"			    { yylval.str=strdup(yytext); count("?"); return('?'); }

[ \t\v\n\f]		{ count("\0"); }
.			    { yyerror("Unidentified token found in the test program !\n");}

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
        else column++;
    }
}