%{
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "nodes.h"
using namespace std;

int yylex(void);
void yyerror(char *s);
FILE *ast;
%}


%union {
  char* str;      /* node label */
  node* ptr;     /* node pointer */
};

%token <str> IDENTIFIER STRING_LITERAL SIZEOF
%token <str> PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token <str> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token <str> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token <str> XOR_ASSIGN OR_ASSIGN TYPE_NAME
%token <str> TYPEDEF EXTERN STATIC AUTO REGISTER
%token <str> CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token <str> STRUCT UNION ENUM ELLIPSIS
%token <str> CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
%token <str> '>' '<' '&' '|' '^' '=' '*' ',' ';'
%token <str> CONSTANT

%start translation_unit 

%type <ptr> multiplicative_expression additive_expression cast_expression primary_expression expression
%type <ptr> type_name assignment_expression postfix_expression argument_expression_list initializer_list unary_expression
%type <ptr> unary_operator shift_expression equality_expression and_expression exclusive_or_expression inclusive_or_expression
%type <ptr> logical_or_expression logical_and_expression conditional_expression assignment_operator declaration constant_expression declaration_specifiers
%type <ptr> init_declarator_list storage_class_specifier type_specifier type_qualifier 
%type <ptr> declarator initializer struct_or_union_specifier enum_specifier struct_or_union struct_declaration_list
%type <ptr> struct_declaration specifier_qualifier_list struct_declarator_list struct_declarator enumerator pointer
%type <ptr> direct_declarator type_qualifier_list parameter_type_list identifier_list parameter_list parameter_declaration  
%type <ptr> abstract_declarator direct_abstract_declarator labeled_statement compound_statement expression_statement declaration_list
%type <ptr> selection_statement iteration_statement jump_statement external_declaration translation_unit function_definition statement
%type <ptr> relational_expression init_declarator statement_list enumerator_list

%%

primary_expression
  : IDENTIFIER                    {$$ = terminal($1); }
  | CONSTANT                      {$$ = terminal("CONSTANT"); }
  | STRING_LITERAL                {$$ = terminal("STRING_LITERAL"); }
  | '(' expression ')'            {$$ = $2; }
  ;

postfix_expression
  : primary_expression                       {$$ = $1; }
  | postfix_expression '[' expression ']'    {$$ = non_terminal(0, "postfix_expression[expression]", $1, $3);}
  | postfix_expression '(' ')'               {$$ = $1;}
  | postfix_expression '(' argument_expression_list ')'   {$$ = non_terminal(0, "postfix_expression(argument_expression_list)", $1, $3);}
  | postfix_expression '.' IDENTIFIER        {$$ = non_terminal(0, "postfix_expression.IDENTIFIER", $1, terminal($3));}
  | postfix_expression PTR_OP IDENTIFIER     {$$ = non_terminal(0, $2, $1, terminal($3));}
  | postfix_expression INC_OP                {$$ = non_terminal(0, $2, $1);}
  | postfix_expression DEC_OP                {$$ = non_terminal(0, $2, $1);}
  ;

argument_expression_list
  : assignment_expression            {$$ = $1; }
  | argument_expression_list ',' assignment_expression    {$$ = non_terminal(0, "argument_expression_list", $1, $3); }
  ;

unary_expression
  : postfix_expression            {$$ = $1; }
  | INC_OP unary_expression       {$$ = non_terminal(0, $1, $2); }

  | DEC_OP unary_expression       {$$ = non_terminal(0, $1, $2); }
  | unary_operator cast_expression{$$ = non_terminal(0, "unary_expression", $1, $2); }
  | SIZEOF unary_expression       {$$ = non_terminal(0, $1, $2); }
  | SIZEOF '(' type_name ')'      {$$ = non_terminal(0, $1, $3); }
  ;

unary_operator
  : '&'      { $$ = terminal("&");}
  | '*'      { $$ = terminal("*");}
  | '+'      { $$ = terminal("+");}
  | '-'      { $$ = terminal("-");}
  | '~'      { $$ = terminal("~");}
  | '!'      { $$ = terminal("!");}
  ;

cast_expression
  : unary_expression                  {$$ = $1; }
  | '(' type_name ')' cast_expression {$$ = non_terminal(0, "cast_expression", $2, $4); }
  ;

multiplicative_expression
  : cast_expression                                     {$$ = $1; }
  | multiplicative_expression '*' cast_expression       {$$ = non_terminal(0, "*", $1, $3); }
  | multiplicative_expression '/' cast_expression       {$$ = non_terminal(0, "/", $1, $3); }
  | multiplicative_expression '%' cast_expression       {$$ = non_terminal(0, "%", $1, $3); }
  ;

additive_expression
  : multiplicative_expression                           {$$ = $1; }
  | additive_expression '+' multiplicative_expression   {$$ = non_terminal(0, "+", $1, $3); }
  | additive_expression '-' multiplicative_expression   {$$ = non_terminal(0, "-", $1, $3); }
  ;


shift_expression
  : additive_expression                           {$$ = $1; }
  | shift_expression LEFT_OP additive_expression  {$$ = non_terminal(0, $2, $1, $3); }
  | shift_expression RIGHT_OP additive_expression {$$ = non_terminal(0, $2, $1, $3); }
  ;

relational_expression
  : shift_expression                              {$$ = $1;}
  | relational_expression '<' shift_expression    {$$ = non_terminal(0, $2, $1, $3); }
  | relational_expression '>' shift_expression    {$$ = non_terminal(0, $2, $1, $3); }
  | relational_expression LE_OP shift_expression  {$$ = non_terminal(0, $2, $1, $3); }
  | relational_expression GE_OP shift_expression  {$$ = non_terminal(0, $2, $1, $3); }
  ;

equality_expression
  : relational_expression                           {$$ = $1;}
  | equality_expression EQ_OP relational_expression {$$ = non_terminal(0, $2, $1, $3); }
  | equality_expression NE_OP relational_expression {$$ = non_terminal(0, $2, $1, $3); }
  ;

and_expression
  : equality_expression                     {$$ = $1;}
  | and_expression '&' equality_expression  {$$ = non_terminal(0, $2, $1, $3); }
  ;

exclusive_or_expression
  : and_expression                              {$$ = $1;}
  | exclusive_or_expression '^' and_expression  {$$ = non_terminal(0, $2, $1, $3); }
  ;

inclusive_or_expression
  : exclusive_or_expression                              {$$ = $1;}
  | inclusive_or_expression '|' exclusive_or_expression  {$$ = non_terminal(0, $2, $1, $3); }
  ;

logical_and_expression
  : inclusive_or_expression                              {$$ = $1;}
  | logical_and_expression AND_OP inclusive_or_expression{$$ = non_terminal(0, $2, $1, $3); }
  ;

logical_or_expression
  : logical_and_expression                               {$$ = $1;}
  | logical_or_expression OR_OP logical_and_expression   {$$ = non_terminal(0, $2, $1, $3); }
  ;

conditional_expression
  : logical_or_expression                                            {$$ = $1;}
  | logical_or_expression '?' expression ':' conditional_expression  {$$ = non_terminal(3, "conditional_expression", $1, $3, $5);}
  ;

assignment_expression
  : conditional_expression                                      {$$ = $1;}
  | unary_expression assignment_operator assignment_expression  {$$ = non_terminal(3, "assignment_expression", $1, $2, $3);}
  ;

assignment_operator
  : '='         {$$ = terminal($1);}
  | MUL_ASSIGN  {$$ = terminal($1);}
  | DIV_ASSIGN  {$$ = terminal($1);}
  | MOD_ASSIGN  {$$ = terminal($1);}
  | ADD_ASSIGN  {$$ = terminal($1);}
  | SUB_ASSIGN  {$$ = terminal($1);}
  | LEFT_ASSIGN {$$ = terminal($1);}
  | RIGHT_ASSIGN{$$ = terminal($1);}
  | AND_ASSIGN  {$$ = terminal($1);}
  | XOR_ASSIGN  {$$ = terminal($1);}
  | OR_ASSIGN   {$$ = terminal($1);}
  ;

expression
  : assignment_expression                 {$$ = $1;}
  | expression ',' assignment_expression  {$$ = non_terminal(0, "expression", $1, $3);}
  ;

constant_expression
  : conditional_expression  {$$ = $1;}
  ;

declaration
  : declaration_specifiers ';'                        {$$ = $1;}
  | declaration_specifiers init_declarator_list ';'   {$$ = non_terminal(0, "declaration", $1, $2);}
  ;

declaration_specifiers
  : storage_class_specifier                           {$$ = $1;}
  | storage_class_specifier declaration_specifiers    {$$ = non_terminal(0, "declaration_specifiers", $1, $2);}  
  | type_specifier                                    {$$ = $1;}
  | type_specifier declaration_specifiers             {$$ = non_terminal(0, "declaration_specifiers", $1, $2);}
  | type_qualifier                                    {$$ = $1;}
  | type_qualifier declaration_specifiers             {$$ = non_terminal(0, "declaration_specifiers", $1, $2);}
  ;

init_declarator_list
  : init_declarator                           {$$ = $1;}
  | init_declarator_list ',' init_declarator  {$$ = non_terminal(0, "init_declarator_list", $1, $3);}
  ;

init_declarator
  : declarator                  {$$ = $1;}
  | declarator '=' initializer  {$$ = non_terminal(0, $2, $1, $3);}
  ;

storage_class_specifier
  : TYPEDEF       {$$ = terminal($1);}
  | EXTERN        {$$ = terminal($1);}
  | STATIC        {$$ = terminal($1);}
  | AUTO          {$$ = terminal($1);}
  | REGISTER      {$$ = terminal($1);}
  ;

type_specifier
  : VOID     {$$ = terminal($1);}
  | CHAR     {$$ = terminal($1);}
  | SHORT    {$$ = terminal($1);}
  | INT      {$$ = terminal($1);}
  | LONG     {$$ = terminal($1);}
  | FLOAT    {$$ = terminal($1);}
  | DOUBLE   {$$ = terminal($1);}
  | SIGNED   {$$ = terminal($1);}
  | UNSIGNED {$$ = terminal($1);}
  | struct_or_union_specifier  {$$ = $1;}
  | enum_specifier  {$$ = $1;}
  | TYPE_NAME       {$$ = terminal($1);}
  ;

struct_or_union_specifier
  : struct_or_union IDENTIFIER '{' struct_declaration_list '}'  {$$ = non_terminal(3, "struct_or_union_specifier", $1, $4, terminal($2));}
  | struct_or_union '{' struct_declaration_list '}'             {$$ = non_terminal(0, "struct_or_union_specifier", $1, $3);}
  | struct_or_union IDENTIFIER                                  {$$ = non_terminal(0, "struct_or_union_specifier", $1, terminal($2));}
  ;

struct_or_union
  : STRUCT    {$$ = terminal($1);}
  | UNION     {$$ = terminal($1);}
  ;

struct_declaration_list
  : struct_declaration                          {$$ = $1;}
  | struct_declaration_list struct_declaration  {$$ = non_terminal(0, "struct_declaration_list", $1, $2);}
  ;

struct_declaration
  : specifier_qualifier_list struct_declarator_list ';' {$$ = non_terminal(0, "struct_declaration", $1, $2);}
  ;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list   {$$ = non_terminal(0, "specifier_qualifier_list", $1, $2);}
	| type_specifier                            {$$ = $1;}
	| type_qualifier specifier_qualifier_list   {$$ = non_terminal(0, "specifier_qualifier_list", $1, $2);}
	| type_qualifier                            {$$ = $1;}
	;

struct_declarator_list
	: struct_declarator                             {$$ = $1;}
	| struct_declarator_list ',' struct_declarator  {$$ = non_terminal(0, "struct_declarator_list", $1, $3);}
	;

struct_declarator
  : declarator                          {$$ = $1;}
	| ':' constant_expression             {$$ = $2;}
	| declarator ':' constant_expression  {$$ = non_terminal(0, "struct_declarator", $1, $3);}
	;

enum_specifier
	: ENUM '{' enumerator_list '}'              {$$ = non_terminal(0, "enum_specifier", $3, NULL, NULL, NULL, NULL, $1);}
	| ENUM IDENTIFIER '{' enumerator_list '}'   {$$ = non_terminal(0, "enum_specifier", $4, terminal($2), NULL, NULL, NULL, $1);}

	| ENUM IDENTIFIER                           {$$ = non_terminal(0, "enum_specifier", terminal($2), NULL, NULL, NULL, NULL, $1);}

	;

enumerator_list
	: enumerator                      {$$ = $1;}
	| enumerator_list ',' enumerator  {$$ = non_terminal(0, "enumerator_list", $1, $3);}
	;

enumerator	
	: IDENTIFIER                          {$$ = terminal($1);}
    | IDENTIFIER '=' constant_expression  {$$ = non_terminal(0, $2, terminal($1), $3);}
	;

type_qualifier
	: CONST     {$$ = terminal($1);}
	| VOLATILE  {$$ = terminal($1);}
	;

declarator
	: pointer direct_declarator {$$ = non_terminal(0, "declarator", $1, $2);}
	| direct_declarator         {$$ = $1;}
	;

direct_declarator
	: IDENTIFIER                                      {$$ = terminal($1);}
	| '(' declarator ')'                              {$$ = $2;}
    | direct_declarator '[' constant_expression ']'   {$$ = non_terminal(0, "direct_declarator", $1, $3, NULL, NULL, NULL, "[]");}
	| direct_declarator '[' ']'                       {$$ = non_terminal(0, "direct_declarator", $1, NULL, NULL, NULL, NULL, "[]");} 
	| direct_declarator '(' parameter_type_list ')'   {$$ = non_terminal(0, "direct_declarator", $1, $3, NULL, NULL, NULL, "()");}
    | direct_declarator '(' identifier_list ')'       {$$ = non_terminal(0, "direct_declarator", $1, $3, NULL, NULL, NULL, "()");}
	| direct_declarator '(' ')'                       {$$ = non_terminal(0, "direct_declarator", $1, NULL, NULL, NULL, NULL, "()");}
	;

pointer
  : '*'                             {$$ = terminal($1);}
  | '*' type_qualifier_list         {$$ = non_terminal(0, $1, $2);}
  | '*' pointer                     {$$ = non_terminal(0, $1, $2);}
	| '*' type_qualifier_list pointer {$$ = non_terminal(0, $1, $2, $3);}
	;


type_qualifier_list
	: type_qualifier                      {$$=$1;}
	| type_qualifier_list type_qualifier  {$$=non_terminal(0, "type_qualifier_list", $1, $2);}
	;

parameter_type_list
	: parameter_list                {$$ = $1;}
	| parameter_list ',' ELLIPSIS   {$$ = non_terminal(0, "parameter_type_list", $1, terminal($3)); }
	;

parameter_list
	: parameter_declaration                     {$$ = $1; }
	| parameter_list ',' parameter_declaration  {$$ = non_terminal(0, "parameter_list", $1, $3); }
	;

parameter_declaration
	: declaration_specifiers declarator             {$$ = non_terminal(0, "parameter_declaration", $1, $2); }
	| declaration_specifiers abstract_declarator    {$$ = non_terminal(0, "parameter_declaration", $1, $2); }
	| declaration_specifiers                        {$$ = $1; }
	;

identifier_list
	: IDENTIFIER                        {$$ = terminal($1); }
	| identifier_list ',' IDENTIFIER    {$$ = non_terminal(0, "identifier_list", $1, terminal($3)); }
	;

type_name
	: specifier_qualifier_list                      {$$ = $1; }
	| specifier_qualifier_list abstract_declarator  {$$ = non_terminal(0, "type_name", $1, $2); }
	;

abstract_declarator
	: pointer                               {$$ = $1; }
	| direct_abstract_declarator            {$$ = $1; }
	| pointer direct_abstract_declarator    {$$ = non_terminal(0, "abstract_declarator", $1, $2); }
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'                            {$$ = $2; }
	| '[' ']'                                                {$$ = terminal("[]"); }
	| '[' constant_expression ']'                            {$$ = $2; }
	| direct_abstract_declarator '[' ']'                     {$$ = non_terminal(0, "direct_abstract_declarator", $1, NULL, NULL, NULL, NULL, "[]"); }
	| direct_abstract_declarator '[' constant_expression ']' {$$ = non_terminal(0, "direct_abstract_declarator", $1, $3, NULL, NULL, NULL, "[]"); }
	| '(' ')'                                                {$$ = terminal("()"); }
	| '(' parameter_type_list ')'                            {$$ = $2; }
	| direct_abstract_declarator '(' ')'                     {$$ = non_terminal(0, "direct_abstract_declarator", $1, NULL, NULL, NULL, NULL, "()"); }
	| direct_abstract_declarator '(' parameter_type_list ')' {$$ = non_terminal(0, "direct_abstract_declarator", $1, $3, NULL, NULL, NULL, "()"); }
	;

initializer
	: assignment_expression         {$$ = $1; }
	| '{' initializer_list '}'      {$$ = $2; }
	| '{' initializer_list ',' '}'  {$$ = non_terminal(0, "initializer", $2, NULL, NULL, NULL, NULL, $3); }
	;

initializer_list
	: initializer                       {$$ = $1; }
	| initializer_list ',' initializer  {$$ = non_terminal(0, "initializer_list", $1, $3); }
	;

statement
	: labeled_statement     {$$ = $1; }
	| compound_statement    {$$ = $1; }
	| expression_statement  {$$ = $1; }
	| selection_statement   {$$ = $1; }
	| iteration_statement   {$$ = $1; }
	| jump_statement        {$$ = $1; }
	;

labeled_statement
	: IDENTIFIER ':' statement                  {$$ = non_terminal(0, "labeled_statement", terminal($1), $3); }
	| CASE constant_expression ':' statement    {$$ = non_terminal(3, "labeled_statement", terminal($1), $2, $4); }
	| DEFAULT ':' statement                     {$$ = non_terminal(0, "labeled_statement", terminal($1), $3); }
	;

compound_statement
	: '{' '}'                                   {$$ = terminal("{}"); }
	| '{' statement_list '}'                    {$$ = $2; }
	| '{' declaration_list '}'                  {$$ = $2; }
	| '{' declaration_list statement_list '}'   {$$ = non_terminal(0, "compound_statement", $2, $3); }
	;

declaration_list
	: declaration                   {$$ = $1; }
	| declaration_list declaration  {$$ = non_terminal(0, "declaration_list", $1, $2);}
	;

statement_list
	: statement                 {$$ = $1; }
	| statement_list statement  {$$ = non_terminal(0, "statement_list", $1, $2);}
	;

expression_statement
	: ';'               {$$ = terminal($1); }
	| expression ';'    {$$ = $1; }
	;

selection_statement
	: IF '(' expression ')' statement                  {$$ = non_terminal(0, "IF (expr) stmt", $3, $5); }
	| IF '(' expression ')' statement ELSE statement   {$$ = non_terminal(3, "IF (expr) stmt ELSE stmt", $3, $5, $7); }
	| SWITCH '(' expression ')' statement              {$$ = non_terminal(0, "SWITCH (expr) stmt", $3, $5); }
	;

iteration_statement
	: WHILE '(' expression ')' statement                                            {$$ = non_terminal(0, "WHILE (expr) stmt", $3, $5);}
	| DO statement WHILE '(' expression ')' ';'                                     {$$ = non_terminal(0, "DO stmt WHILE (expr)", $2, $5);}
	| FOR '(' expression_statement expression_statement ')' statement               {$$ = non_terminal(3, "FOR (expr_stmt expr_stmt) stmt", $3, $4, $6);}
	| FOR '(' expression_statement expression_statement expression ')' statement    {$$ = non_terminal(3, "FOR (expr_stmt expr_stmt expr) stmt", $3, $4, $5, $7);}
	;

jump_statement
	: GOTO IDENTIFIER ';'       {$$ = non_terminal(0, "jump_stmt", terminal($1), terminal($2)); }
	| CONTINUE ';'              {$$ = terminal($1); }
	| BREAK ';'                 {$$ = terminal($1); }
	| RETURN ';'                {$$ = terminal($1); }
	| RETURN expression ';'     {$$ = non_terminal(0, "jump_stmt", terminal($1), $2); }
	;

translation_unit
	: external_declaration                  {$$ = $1; }
	| translation_unit external_declaration {$$ = non_terminal(0, "translation_unit", $1, $2); }
	;

external_declaration
	: function_definition   {$$ = $1; }
	| declaration           {$$ = $1; }
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement {$$ = non_terminal(3, "function_definition", $1, $2, $3, $4); }
	| declaration_specifiers declarator compound_statement                  {$$ = non_terminal(3, "function_definition", $1, $2, $3); }
	| declarator declaration_list compound_statement                        {$$ = non_terminal(3, "function_definition", $1, $2, $3); }
	| declarator compound_statement                                         {$$ = non_terminal(0, "function_definition", $1, $2); }
	;

%%

void yyerror(char* s){
    fprintf(stderr, "%s\n", s);
}

void usage_info(){
    printf("Specify -help for usage info\n");
    printf("Specify test program with -i flag\n");
    printf("Specify output file with -o flag\n");
    return;
}

extern FILE* yyin;
int main (int argc, char* argv[]){
    if(argc == 1){
        usage_info();
        return 0;
    }
    yyin = NULL;
    ast = NULL;
    for(int i = 0; i < argc-1; i++){
        if(!strcmp(argv[i+1], "-help")){
            usage_info();
            return 0;
        }
        if(i == argc-2){
            usage_info();
            return 0;
        }
        if(!strcmp(argv[i+1], "-i")){
            yyin = fopen (argv[i+2], "r");
            i++;
        }else if(!strcmp(argv[i+1], "-o")){
            ast = fopen(argv[i+2], "w");
            i++;
        }else{
            usage_info();
            return 0;
        }
    }
    if(yyin == NULL){
        printf("Please specify a valid input file\n");
        return 0;
    }
    if (ast == NULL){
        printf("Please specify an output file\n");
        return 0;
    }
    graph_init();
    yyparse();
    graph_end();
    fclose (yyin);
    fclose (ast);
    return 0;
}