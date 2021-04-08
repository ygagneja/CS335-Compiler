%{
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "nodes.h"
#include "sym_table.h"
#include "type_check.h"

#define MAX_LEVELS 1024
using namespace std;

bool error_throw = false;
extern sym_tab* curr;
string curr_args_types;
string func_name;
string func_type;
string func_args;
string t_name = "";
int struct_id = 0;
int level_id[MAX_LEVELS];
int level = 0;

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

%type <str> empty1 empty2 op_brace cl_brace
%type <ptr> multiplicative_expression additive_expression cast_expression primary_expression expression
%type <ptr> type_name assignment_expression postfix_expression argument_expression_list initializer_list unary_expression
%type <ptr> unary_operator shift_expression equality_expression and_expression exclusive_or_expression inclusive_or_expression
%type <ptr> logical_or_expression logical_and_expression conditional_expression assignment_operator declaration constant_expression declaration_specifiers
%type <ptr> init_declarator_list storage_class_specifier type_specifier type_qualifier 
%type <ptr> declarator initializer struct_or_union_specifier enum_specifier struct_or_union struct_declaration_list
%type <ptr> struct_declaration specifier_qualifier_list struct_declarator_list struct_declarator enumerator pointer
%type <ptr> direct_declarator type_qualifier_list parameter_type_list identifier_list parameter_list parameter_declaration  
%type <ptr> labeled_statement compound_statement expression_statement declaration_list
%type <ptr> selection_statement iteration_statement jump_statement external_declaration translation_unit function_definition statement
%type <ptr> relational_expression init_declarator statement_list enumerator_list

%%

primary_expression
  : IDENTIFIER                    {$$ = terminal($1);
                                    string type = id_type($1);
                                    if (type != ""){
                                      $$->init = lookup($1, level, level_id)->init;
                                      $$->nodetype = type;
                                      $$->symbol = string($1);
                                      $$->expr_type = 3;
                                    }
                                    else {
                                      error_throw = true;
                                      $$->nodetype = type;
                                      fprintf(stderr, "Error : %s is not declared in this scope", $1);
                                    }
                                  }
  | CONSTANT                      {$$ = terminal("CONSTANT");
                                    $$->init = true;
                                    $$->nodetype = string("long double");
                                    $$->expr_type = 5;
                                  }
  | STRING_LITERAL                {$$ = terminal("STRING_LITERAL");
                                    $$->nodetype = string("char*");
                                    $$->init = true;
                                  }
  | '(' expression ')'            {$$ = $2; }
  ;

postfix_expression
  : primary_expression                       {$$ = $1; }
  | postfix_expression '[' expression ']'    {$$ = non_terminal(0, "postfix_expression[expression]", $1, $3);
                                              if ($1->init && $3->init){
                                                $$->init = true;
                                              }
                                              string type = postfix_type($1->nodetype, 1);
                                              if (type != ""){
                                                $$->nodetype = type;
                                              }
                                              else {
                                                error_throw = true;
                                                $$->nodetype = type;
                                                fprintf(stderr,"Error : Array indexed with more indices than its dimension");
                                              }
                                             }
  | postfix_expression '(' ')'               {$$ = $1;
                                              $$->init = true;
                                              string type = postfix_type($1->nodetype, 2);
                                              $$->nodetype = string(type);
                                              if (type != ""){
                                                if ($1->expr_type == 3){
                                                  string args = get_func_args($1->symbol);
                                                  if (args != ""){
                                                    error_throw = true;
                                                    fprintf(stderr, "Error : %s function requires arguments to be passed", ($1->symbol).c_str());
                                                  }
                                                }
                                              }
                                              else {
                                                error_throw = true;
                                                fprintf(stderr, "Error : Invalid function call");
                                              }
                                              curr_args_types = string("");
                                             }
  | postfix_expression '(' argument_expression_list ')'   {$$ = non_terminal(0, "postfix_expression(argument_expression_list)", $1, $3);
                                                            if ($3->init) $$->init = true;
                                                            string type = postfix_type($1->nodetype, 3);
                                                            $$->nodetype = string(type);
                                                            if (type != ""){
                                                              if ($1->expr_type == 3){
                                                                string args = get_func_args($1->symbol);
                                                                string temp1 = curr_args_types;
                                                                string temp2 = args;
                                                                string typeA, typeB;
                                                                string delim = string(",");
                                                                unsigned f1 = 1;
                                                                unsigned f2 = 1;
                                                                int arg_num = 0;
                                                                while(f1 != -1 && f2 != -1){
                                                                  f1 = temp1.find_first_of(delim);
                                                                  f2 = temp2.find_first_of(delim);
                                                                  arg_num++;
                                                                  if (f1 == -1) typeA = temp1; 
                                                                  else { 
                                                                    typeA = temp1.substr(0,f1); 
                                                                    temp1 = temp1.substr(f1+1);
                                                                  }
                                                                  if (f2 == -1) typeB = temp2; 
                                                                  else { 
                                                                    typeB = temp2.substr(0,f2); 
                                                                    temp2 = temp2.substr(f2+1); 
                                                                  }
                                                                  if (typeB == string("...")) break;
                                                                  string chk = is_valid(typeA, typeB);
                                                                  if (chk == string("0")){
                                                                    fprintf(stderr, "Warning : Passing argument %d of %s from incompatible pointer type. Note : expected %s but argument is of type %s", arg_num, ($1->symbol).c_str(), typeB.c_str(), typeA.c_str());
                                                                  }
                                                                  else if (chk == ""){
                                                                    error_throw = true;
                                                                    fprintf(stderr, "Error : Incompatible type for argument %d of %s. Note : expected %s but argument is of type %s", arg_num, ($1->symbol).c_str(), typeB.c_str(), typeA.c_str());
                                                                  }
                                                                  if ((f1 != -1) && (f2 != -1)){
                                                                    continue;
                                                                  }
                                                                  else if (f2 != -1){
                                                                    if(!(temp2==string("..."))){
                                                                      error_throw = true;
                                                                      fprintf(stderr, "Too few arguments for the function %s", ($1->symbol).c_str());
                                                                    } 
                                                                    break;
                                                                  }
                                                                  else if (f1 != -1){
                                                                    error_throw = true;
                                                                    fprintf(stderr, "Too many arguments for the function %s", ($1->symbol).c_str());
                                                                    break;
                                                                  }
                                                                  else break;  
                                                              }
                                                            }
                                                            else {
                                                              error_throw = true;
                                                              fprintf(stderr, "Error : Invalid function call %s", ($1->symbol).c_str());
                                                            }
                                                          }
  | postfix_expression '.' IDENTIFIER        {$$ = non_terminal(0, "postfix_expression.IDENTIFIER", $1, terminal($3));
                                              // what is this
                                             }
  | postfix_expression PTR_OP IDENTIFIER     {$$ = non_terminal(0, $2, $1, terminal($3));
                                              // what is this
                                             }
  | postfix_expression INC_OP                {$$ = non_terminal(0, $2, $1);
                                              if ($1->init) $$->init = true;
                                              string type = postfix_type($1->nodetype, 6);
                                              $$->nodetype = string(type);
                                              if (type == ""){
                                                error_throw = true;
                                                fprintf(stderr, "Error : Increment used with incompatible type");
                                              }
                                             }
  | postfix_expression DEC_OP                {$$ = non_terminal(0, $2, $1);
                                              if ($1->init) $$->init = true;
                                              string type = postfix_type($1->nodetype, 7);
                                              $$->nodetype = string(type);
                                              if (type == ""){
                                                error_throw = true;
                                                fprintf(stderr, "Error : Decrement used with incompatible type");
                                              }
                                             }
  ;

argument_expression_list
  : assignment_expression            {$$ = $1; 
                                      curr_args_types = $$->nodetype;
                                      if ($$->nodetype != "") $$->nodetype = "void";
                                     }
  | argument_expression_list ',' assignment_expression    {$$ = non_terminal(0, "argument_expression_list", $1, $3); 
                                                          string type = args_type($1->nodetype, $3->nodetype);
                                                          $$->nodetype = string(type);
                                                          if ($1->init && $3->init) $$->init = true;
                                                          curr_args_types = curr_args_types + string(",") + $3->nodetype;
                                                          }
  ;

unary_expression
  : postfix_expression            {$$ = $1; }
  | INC_OP unary_expression       {$$ = non_terminal(0, $1, $2);
                                    if ($2->init) $$->init = true;
                                    string type = postfix_type($2->nodetype, 6);
                                    $$->nodetype = type;
                                    if (type == ""){
                                      error_throw = true;
                                      fprintf(stderr, "Increment used with incompatible type");
                                    }
                                  }

  | DEC_OP unary_expression       {$$ = non_terminal(0, $1, $2); 
                                    if ($2->init) $$->init = true;
                                    string type = postfix_type($2->nodetype, 7);
                                    $$->nodetype = type;
                                    if (type == ""){
                                      error_throw = true;
                                      fprintf(stderr, "Decrement used with incompatible type");
                                    }
                                  }
  | unary_operator cast_expression{$$ = non_terminal(0, "unary_expression", $1, $2); 
                                    if ($2->init) $$->init = true;
                                    string type = unary_type($1->label, $2->nodetype);
                                    $$->nodetype = type;
                                    if (type == ""){
                                      error_throw = true;
                                      fprintf(stderr, "Type inconsistent with %s operator", ($1->label).c_str());
                                    }
                                  }
  | SIZEOF unary_expression       {$$ = non_terminal(0, $1, $2); 
                                    if ($2->init) $$->init = true;
                                    $$->nodetype = string("unsigned int");
                                    string type = $2->nodetype;
                                    if (type == ""){
                                      error_throw = true;
                                      fprintf(stderr, "Error : sizeof cannot be defined for given identifiers");
                                    }
                                  }
  | SIZEOF '(' type_name ')'      {$$ = non_terminal(0, $1, $3); 
                                    if ($3->init) $$->init = true;
                                    $$->nodetype = string("unsigned int");
                                    string type = $3->nodetype;
                                    if (type == ""){
                                      error_throw = true;
                                      fprintf(stderr, "Error : sizeof cannot be defined for given identifiers");
                                    }
                                  }
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
  | '(' type_name ')' cast_expression {$$ = non_terminal(0, "cast_expression", $2, $4); 
                                        string type = cast_type($2->nodetype, $4->nodetype);
                                        $$->nodetype = type;
                                        if (type == string("0")){
                                          $$->nodetype = $2->nodetype;
                                          fprintf(stderr, "Warning : Incompatible pointer typecasting");
                                        }
                                        else if (type == string("1")){
                                          $$->nodetype = $2->nodetype;
                                        }
                                        else {
                                          error_throw = true;
                                          fprintf(stderr, "Error : Cannot typecast given expression to %s", ($2->nodetype).c_str());
                                        }
                                        if ($4->init) $$->init = true;
                                      }
  ;

multiplicative_expression
  : cast_expression                                     {$$ = $1; }
  | multiplicative_expression '*' cast_expression       { if ($1->init && $3->init) $$->init = true;
                                                          string type = mul_type($1->nodetype, $3->nodetype, '*');
                                                          if (type != ""){
                                                            if (type == string("int")){
                                                              $$ = non_terminal(0, "* int", $1, $3);
                                                              $$->nodetype = string("long long");
                                                            }
                                                            else if (type == string("float")){
                                                              $$ = non_terminal(0, "* float", $1, $3);
                                                              $$->nodetype = string("long double");
                                                            }
                                                          }
                                                          else {
                                                            error_throw = true;
                                                            fprintf(stderr, "Error : Incompatible type for * operator");
                                                            $$ = non_terminal(0, "*", $1, $3);
                                                            $$->nodetype = string(""); 
                                                          }
                                                        }
  | multiplicative_expression '/' cast_expression       {if ($1->init && $3->init) $$->init = true;
                                                          string type = mul_type($1->nodetype, $3->nodetype, '/');
                                                          if (type != ""){
                                                            if (type == string("int")){
                                                              $$ = non_terminal(0, "/ int", $1, $3);
                                                              $$->nodetype = string("long long");
                                                            }
                                                            else if (type == string("float")){
                                                              $$ = non_terminal(0, "/ float", $1, $3);
                                                              $$->nodetype = string("long double");
                                                            }
                                                          }
                                                          else {
                                                            error_throw = true;
                                                            fprintf(stderr, "Error : Incompatible type for / operator");
                                                            $$ = non_terminal(0, "/", $1, $3);
                                                            $$->nodetype = string(""); 
                                                          } 
                                                        }
  | multiplicative_expression '%' cast_expression       {if ($1->init && $3->init) $$->init = true;
                                                          string type = mul_type($1->nodetype, $3->nodetype, '%');
                                                          if (type != ""){
                                                            $$ = non_terminal(0, "% int", $1, $3);
                                                            $$->nodetype = string("long long");
                                                          }
                                                          else {
                                                            error_throw = true;
                                                            fprintf(stderr, "Error : Incompatible type for % operator");
                                                            $$ = non_terminal(0, "%", $1, $3);
                                                            $$->nodetype = string(""); 
                                                          } 
                                                        }
  ;

additive_expression
  : multiplicative_expression                           {$$ = $1; }
  | additive_expression '+' multiplicative_expression   {if ($1->init && $3->init) $$->init = true;
                                                          string type = add_type($1->nodetype, $3->nodetype);
                                                          if (type != ""){
                                                            if (type == string("int")){
                                                              $$->nodetype = string("long long");
                                                              $$ = non_terminal(0, "+ " + type, $1, $3);
                                                            }
                                                            else if (type == string("float")){
                                                              $$->nodetype = string("long double");
                                                              $$ = non_terminal(0, "+ " + type, $1, $3);
                                                            }
                                                            else {
                                                              $$->nodetype = type;
                                                              $$ = non_terminal(0, "+ " + type, $1, $3);
                                                            }
                                                          }
                                                          else {
                                                            error_throw = true;
                                                            fprintf(stderr, "Error : Incompatible type for + operator");
                                                            $$ = non_terminal(0, "+", $1, $3);
                                                            $$->nodetype = string("");
                                                          }
                                                        }
  | additive_expression '-' multiplicative_expression   {if ($1->init && $3->init) $$->init = true;
                                                          string type = add_type($1->nodetype, $3->nodetype);
                                                          if (type != ""){
                                                            if (type == string("int")){
                                                              $$->nodetype = string("long long");
                                                              $$ = non_terminal(0, "- " + type, $1, $3);
                                                            }
                                                            else if (type == string("float")){
                                                              $$->nodetype = string("long double");
                                                              $$ = non_terminal(0, "- " + type, $1, $3);
                                                            }
                                                            else {
                                                              $$->nodetype = type;
                                                              $$ = non_terminal(0, "- " + type, $1, $3);
                                                            }
                                                          }
                                                          else {
                                                            error_throw = true;
                                                            fprintf(stderr, "Error : Incompatible type for - operator");
                                                            $$ = non_terminal(0, "-", $1, $3);
                                                            $$->nodetype = string("");
                                                          }
                                                        }
  ;


shift_expression
  : additive_expression                           {$$ = $1; }
  | shift_expression LEFT_OP additive_expression  {$$ = non_terminal(0, $2, $1, $3);
                                                    if ($1->init && $3->init) $$->init = true;
                                                    string type = shift_type($1->nodetype, $3->nodetype);
                                                    $$->nodetype = type;
                                                    if (type == ""){
                                                      error_throw = true;
                                                      fprintf(stderr, "Error : Invalid operand(s) with <<");
                                                    }
                                                  }
  | shift_expression RIGHT_OP additive_expression {$$ = non_terminal(0, $2, $1, $3); 
                                                    if ($1->init && $3->init) $$->init = true;
                                                    string type = shift_type($1->nodetype, $3->nodetype);
                                                    $$->nodetype = type;
                                                    if (type == ""){
                                                      error_throw = true;
                                                      fprintf(stderr, "Error : Invalid operand(s) with >>");
                                                    }
                                                  }
  ;

relational_expression
  : shift_expression                              {$$ = $1;}
  | relational_expression '<' shift_expression    {if ($1->init && $3->init) $$->init = true;
                                                    string type = relat_type($1->nodetype, $3->nodetype);
                                                    if (type != ""){
                                                      if (type == string("*warning")){
                                                        fprintf(stderr, "Warning : Comparison between %s and %s", ($1->nodetype).c_str(), ($3->nodetype).c_str());
                                                        type = string("*");
                                                      }
                                                      $$->nodetype = string("bool");
                                                      $$ = non_terminal(0, "< " + type, $1, $3);
                                                    }
                                                    else {
                                                      error_throw = true;
                                                      fprintf(stderr, "Invalid operand(s) with <");
                                                      $$->nodetype = type;
                                                      $$ = non_terminal(0, $2, $1, $3);
                                                    }
                                                  }
  | relational_expression '>' shift_expression    {if ($1->init && $3->init) $$->init = true;
                                                    string type = relat_type($1->nodetype, $3->nodetype);
                                                    if (type != ""){
                                                      if (type == string("*warning")){
                                                        fprintf(stderr, "Warning : Comparison between %s and %s", ($1->nodetype).c_str(), ($3->nodetype).c_str());
                                                        type = string("*");
                                                      }
                                                      $$->nodetype = string("bool");
                                                      $$ = non_terminal(0, "> " + type, $1, $3);
                                                    }
                                                    else {
                                                      error_throw = true;
                                                      fprintf(stderr, "Invalid operand(s) with >");
                                                      $$->nodetype = type;
                                                      $$ = non_terminal(0, $2, $1, $3);
                                                    } 
                                                  }
  | relational_expression LE_OP shift_expression  {if ($1->init && $3->init) $$->init = true;
                                                    string type = relat_type($1->nodetype, $3->nodetype);
                                                    if (type != ""){
                                                      if (type == string("*warning")){
                                                        fprintf(stderr, "Warning : Comparison between %s and %s", ($1->nodetype).c_str(), ($3->nodetype).c_str());
                                                        type = string("*");
                                                      }
                                                      $$->nodetype = string("bool");
                                                      $$ = non_terminal(0, "<= " + type, $1, $3);
                                                    }
                                                    else {
                                                      error_throw = true;
                                                      fprintf(stderr, "Invalid operand(s) with <=");
                                                      $$->nodetype = type;
                                                      $$ = non_terminal(0, $2, $1, $3);
                                                    }
                                                  }
  | relational_expression GE_OP shift_expression  {if ($1->init && $3->init) $$->init = true;
                                                    string type = relat_type($1->nodetype, $3->nodetype);
                                                    if (type != ""){
                                                      if (type == string("*warning")){
                                                        fprintf(stderr, "Warning : Comparison between %s and %s", ($1->nodetype).c_str(), ($3->nodetype).c_str());
                                                        type = string("*");
                                                      }
                                                      $$->nodetype = string("bool");
                                                      $$ = non_terminal(0, ">= " + type, $1, $3);
                                                    }
                                                    else {
                                                      error_throw = true;
                                                      fprintf(stderr, "Invalid operand(s) with >=");
                                                      $$->nodetype = type;
                                                      $$ = non_terminal(0, $2, $1, $3);
                                                    }
                                                  }
  ;

equality_expression
  : relational_expression                           {$$ = $1;}
  | equality_expression EQ_OP relational_expression {if ($1->init && $3->init) $$->init = true;
                                                      string type = relat_type($1->nodetype, $3->nodetype);
                                                      if (type != ""){
                                                        if (type == string("*warning")){
                                                        fprintf(stderr, "Warning : Comparison between %s and %s", ($1->nodetype).c_str(), ($3->nodetype).c_str());
                                                        type = string("*");
                                                      }
                                                        $$->nodetype = string("bool");
                                                        $$ = non_terminal(0, "== " + type, $1, $3);
                                                      }
                                                      else {
                                                        error_throw = true;
                                                        fprintf(stderr, "Invalid operand(s) with ==");
                                                        $$->nodetype = type;
                                                        $$ = non_terminal(0, $2, $1, $3);
                                                      } 
                                                    }
  | equality_expression NE_OP relational_expression {if ($1->init && $3->init) $$->init = true;
                                                      string type = relat_type($1->nodetype, $3->nodetype);
                                                      if (type != ""){
                                                        if (type == string("*warning")){
                                                        fprintf(stderr, "Warning : Comparison between %s and %s", ($1->nodetype).c_str(), ($3->nodetype).c_str());
                                                        type = string("*");
                                                      }
                                                        $$->nodetype = string("bool");
                                                        $$ = non_terminal(0, "!= " + type, $1, $3);
                                                      }
                                                      else {
                                                        error_throw = true;
                                                        fprintf(stderr, "Invalid operand(s) with !=");
                                                        $$->nodetype = type;
                                                        $$ = non_terminal(0, $2, $1, $3);
                                                      } 
                                                    }
  ;

and_expression
  : equality_expression                     {$$ = $1;}
  | and_expression '&' equality_expression  {if ($1->init && $3->init) $$->init = true;
                                              string type = bit_type($1->nodetype, $3>nodetype);
                                              if (type != ""){
                                                if (type == string("int")){
                                                  $$ = non_terminal(0, "& int", $1, $3);
                                                  $$->nodetype = string("long long");
                                                }
                                                else {
                                                  $$ = non_terminal(0, "& bool", $1, $3);
                                                  $$->nodetype = type;
                                                }
                                              }
                                              else {
                                                error_throw = true;
                                                fprintf(stderr, "Invalid operand(s) with &");
                                                $$->nodetype = type;
                                                $$ = non_terminal(0, $2, $1, $3);
                                              }
                                            }
  ;

exclusive_or_expression
  : and_expression                              {$$ = $1;}
  | exclusive_or_expression '^' and_expression  {if ($1->init && $3->init) $$->init = true;
                                                  string type = bit_type($1->nodetype, $3>nodetype);
                                                  if (type != ""){
                                                    if (type == string("int")){
                                                      $$ = non_terminal(0, "^ int", $1, $3);
                                                      $$->nodetype = string("long long");
                                                    }
                                                    else {
                                                      $$ = non_terminal(0, "^ bool", $1, $3);
                                                      $$->nodetype = type;
                                                    }
                                                  }
                                                  else {
                                                    error_throw = true;
                                                    fprintf(stderr, "Invalid operand(s) with ^");
                                                    $$->nodetype = type;
                                                    $$ = non_terminal(0, $2, $1, $3);
                                                  } 
                                                }
  ;

inclusive_or_expression
  : exclusive_or_expression                              {$$ = $1;}
  | inclusive_or_expression '|' exclusive_or_expression  {if ($1->init && $3->init) $$->init = true;
                                                          string type = bit_type($1->nodetype, $3>nodetype);
                                                          if (type != ""){
                                                            if (type == string("int")){
                                                              $$ = non_terminal(0, "| int", $1, $3);
                                                              $$->nodetype = string("long long");
                                                            }
                                                            else {
                                                              $$ = non_terminal(0, "| bool", $1, $3);
                                                              $$->nodetype = type;
                                                            }
                                                          }
                                                          else {
                                                            error_throw = true;
                                                            fprintf(stderr, "Invalid operand(s) with |");
                                                            $$->nodetype = type;
                                                            $$ = non_terminal(0, $2, $1, $3);
                                                          } }
  ;

logical_and_expression
  : inclusive_or_expression                              {$$ = $1;}
  | logical_and_expression AND_OP inclusive_or_expression{$$ = non_terminal(0, $2, $1, $3); 
                                                          if ($1->init && $3->init) $$->init = true;
                                                          $$->nodetype = string("bool");
                                                         }
  ;

logical_or_expression
  : logical_and_expression                               {$$ = $1;}
  | logical_or_expression OR_OP logical_and_expression   {$$ = non_terminal(0, $2, $1, $3); 
                                                          if ($1->init && $3->init) $$->init = true;
                                                          $$->nodetype = string("bool");
                                                         }
  ;

conditional_expression
  : logical_or_expression                                            {$$ = $1;}
  | logical_or_expression '?' expression ':' conditional_expression  {$$ = non_terminal(3, "conditional_expression", $1, $3, $5);
                                                                      string type = cond_type($3->nodetype, $5->nodetype);
                                                                      if (type != ""){
                                                                        $$->nodetype = type;
                                                                      }
                                                                      else {
                                                                        error_throw = true;
                                                                        $$->nodetype = type;
                                                                        fprintf(stderr, "Error : Type mismatch in ternary expression");
                                                                      }
                                                                      if ($1->init && $3->init && $5->init) $$->init = true;
                                                                     }
  ;

assignment_expression
  : conditional_expression                                      {$$ = $1;}
  | unary_expression assignment_operator assignment_expression  {$$ = non_terminal(3, "assignment_expression", $1, $2, $3);
                                                                  string type = assign_type($1->nodetype, $3->nodetype, $2->label);
                                                                  if (type == string("0")){
                                                                    $$->nodetype = $1->nodetype;
                                                                    fprintf(stderr, "Warning : Incompatible pointer type assignment");
                                                                  }
                                                                  else if (type == string("1")){
                                                                    $$->nodetype = $1->nodetype;
                                                                  }
                                                                  else {
                                                                    error_throw = true;
                                                                    $$->nodetype = type;
                                                                    fprintf(stderr, "Error : Incompatible type conversion from %s to %s", ($3->nodetype).c_str(), ($1->nodetype).c_str());
                                                                  }
                                                                }
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
  : storage_class_specifier                           {$$ = $1;
                                                        error_throw = true;
                                                        fprintf(stderr, "Error : Storage class specifier not implemented yet");
                                                      }
  | storage_class_specifier declaration_specifiers    {$$ = non_terminal(0, "declaration_specifiers", $1, $2);
                                                        error_throw = true;
                                                        fprintf(stderr, "Error : Storage class specifier not implemented yet");
                                                      }  
  | type_specifier                                    {$$ = $1;}
  | type_specifier declaration_specifiers             {$$ = non_terminal(0, "declaration_specifiers", $1, $2);}
  | type_qualifier                                    {$$ = $1;
                                                        error_throw = true;
                                                        fprintf(stderr, "Error : Type qualifier not implemented yet");
                                                      }
  | type_qualifier declaration_specifiers             {$$ = non_terminal(0, "declaration_specifiers", $1, $2);
                                                        error_throw = true;
                                                        fprintf(stderr, "Error : Type qualifier not implemented yet");
                                                      }
  ;

init_declarator_list
  : init_declarator                           {$$ = $1;}
  | init_declarator_list ',' init_declarator  {$$ = non_terminal(0, "init_declarator_list", $1, $3);}
  ;

init_declarator
  : declarator                  {$$ = $1;
                                  if ($1->expr_type == 1){
                                    string type = $1->nodetype;
                                    if (lookup($1->symbol, level, level_id[level][level])){
                                      error_throw = true;
                                      fprintf(stderr, "Error : Redeclaration of symbol %s", ($1->symbol).c_str());
                                    }
                                    else if ($1->nodetype == string("void")){
                                      error_throw = true;
                                      fprintf(stderr, "Error : Variable or field %s declared as type void", ($1->symbol).c_str());
                                    }
                                    else {
                                      insert_entry($1->symbol, $1->nodetype, $1->size, 0, false, level, level_id[level]);
                                    }
                                  }
                                }
  | declarator '=' initializer  {$$ = non_terminal(0, $2, $1, $3);
                                  if ($1->expr_type == 1 || $1->expr_type == 15){
                                    string type = $1->nodetype;
                                    if (lookup($1->symbol, level, level_id[level])){
                                      error_throw = true;
                                      fprintf(stderr, "Error : Redeclaration of symbol %s", ($1->symbol).c_str());
                                    }
                                    else if ($1->nodetype == string("void")){
                                      error_throw = true;
                                      fprintf(stderr, "Error : Variable or field %s declared as type void", ($1->symbol).c_str());
                                    }
                                    else {
                                      if ($1->expr_type == 15){
                                        // idk
                                      }
                                      insert_entry($1->symbol, $1->nodetype, $1->size, 0, true, level, level_id[level]);
                                    }
                                  }
                                }
  ;

storage_class_specifier
  : TYPEDEF       {$$ = terminal($1);}
  | EXTERN        {$$ = terminal($1);}
  | STATIC        {$$ = terminal($1);}
  | AUTO          {$$ = terminal($1);}
  | REGISTER      {$$ = terminal($1);}
  ;

type_specifier
  : VOID     {$$ = terminal($1); t_name = (t_name=="") ? string($1) : t_name+" "+string($1);}
  | CHAR     {$$ = terminal($1); t_name = (t_name=="") ? string($1) : t_name+" "+string($1);}
  | SHORT    {$$ = terminal($1); t_name = (t_name=="") ? string($1) : t_name+" "+string($1);}
  | INT      {$$ = terminal($1); t_name = (t_name=="") ? string($1) : t_name+" "+string($1);}
  | LONG     {$$ = terminal($1); t_name = (t_name=="") ? string($1) : t_name+" "+string($1);}
  | FLOAT    {$$ = terminal($1); t_name = (t_name=="") ? string($1) : t_name+" "+string($1);}
  | DOUBLE   {$$ = terminal($1); t_name = (t_name=="") ? string($1) : t_name+" "+string($1);}
  | SIGNED   {$$ = terminal($1); t_name = (t_name=="") ? string($1) : t_name+" "+string($1);}
  | UNSIGNED {$$ = terminal($1); t_name = (t_name=="") ? string($1) : t_name+" "+string($1);}
  | struct_or_union_specifier  {$$ = $1; t_name = (t_name=="") ? string($1->nodetype) : t_name+" "+string($1->nodetype);}
  | enum_specifier  {$$ = $1; 
                    error_throw = true;
                    fprintf(stderr, "Error : Enum not implemented yet"); 
                    }
  | TYPE_NAME       {$$ = terminal($1);
                      t_name = (t_name=="") ? string($1) : t_name+" "+string($1);
                    } 
  ;

struct_or_union_specifier
  : struct_or_union IDENTIFIER '{' struct_declaration_list '}'  {$$ = non_terminal(3, "struct_or_union_specifier", $1, $4, terminal($2));
                                                                  if (lookup_type(string($1->label) + " " + string($2))){
                                                                    $$->nodetype = "";
                                                                    error_throw = true;
                                                                    fprintf(stderr, "Error : Struct/Union %s is already defined", $2);
                                                                  }
                                                                  else {
                                                                    $$->nodetype = string($1->label) + " " + string($2);
                                                                    int size = string($1->label) == "struct" ? $4->size : $4->max_size;
                                                                    insert_type_entry($$->nodetype, string($1->label), size, level, level_id[level]);
                                                                  }
                                                                }
  | struct_or_union '{' struct_declaration_list '}'             {$$ = non_terminal(0, "struct_or_union_specifier", $1, $3);
                                                                  struct_id++;
                                                                  $$->nodetype = string($1->label) + " " + to_string(struct_id);
                                                                  int size = string($1->label) == "struct" ? $4->size : $4->max_size;
                                                                  insert_type_entry($$->nodetype, string($1->label), size, level, level_id[level]);
                                                                }
  | struct_or_union IDENTIFIER                                  {$$ = non_terminal(0, "struct_or_union_specifier", $1, terminal($2));
                                                                  if (lookup_type(string($1->label) + " " + string($2))){
                                                                    $$->nodetype = string($1->label) + " " + string($2);
                                                                  }
                                                                  else {
                                                                    $$->nodetype = "";
                                                                    error_throw = true;
                                                                    fprintf(stderr, "Error : Struct/Union %s not defined", $2->label);
                                                                  }
                                                                }
  ;

struct_or_union
  : STRUCT    {$$ = terminal($1);}
  | UNION     {$$ = terminal($1);}
  ;

struct_declaration_list
  : struct_declaration                          {$$ = $1;}
  | struct_declaration_list struct_declaration  {$$ = non_terminal(0, "struct_declaration_list", $1, $2);
                                                  $$->size = $1->size + $2->size;
                                                  $$->max_size = $1->max_size > $2->size ? $1->max_size : $2->size;
                                                }
  ;

struct_declaration
  : specifier_qualifier_list struct_declarator_list ';' {$$ = non_terminal(0, "struct_declaration", $1, $2);
                                                          t_name = string("");
                                                          $$->size = 8;
                                                        }
  ;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list   {$$ = non_terminal(0, "specifier_qualifier_list", $1, $2);}
	| type_specifier                            {$$ = $1;}
	| type_qualifier specifier_qualifier_list   {$$ = non_terminal(0, "specifier_qualifier_list", $1, $2);
                                                error_throw = true;
                                                fprintf(stderr, "Error : Type qualifier not implemented yet");
                                              }
	| type_qualifier                            {$$ = $1;
                                                error_throw = true;
                                                fprintf(stderr, "Error : Type qualifier not implemented yet");
                                              }
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
	: pointer direct_declarator {
                                $$ = non_terminal(0, "declarator", $1, $2);
                                if($2->expr_type == 1){
                                  $$->nodetype = $2->nodetype+$1->nodetype;
                                  $$->symbol = $2->symbol;
                                  $$->expr_type = 1;
                                }
                                if($2->expr_type == 2){
                                  func_name = $2->symbol;
                                  func_type = $2->nodetype;
                                }
                                $$->size = get_size($$->nodetype);
                              }
	| direct_declarator         {
                                $$ = $1;
                                if($1->expr_type == 2){
                                  func_name = $1->symbol;
                                  func_type = $1->nodetype;
                                }
                              }
	;

direct_declarator
	: IDENTIFIER            {
														$$ = terminal($1);
                            $$->expr_type = 1;
														$$->symbol = string($1);
														$$->nodetype = t_name;
														$$->size = get_size(t_name);
													}
	| '(' declarator ')'    {
														$$ = $2;
                            if($2->expr_type == 1){
                              $$->expr_type = 1;
                              $$->symbol = $2->symbol;
														  $$->nodetype = $2->nodetype;
                            }
													}
  | direct_declarator '[' constant_expression ']'   {/*TODO*/ $$ = non_terminal(0, "direct_declarator", $1, $3, NULL, NULL, NULL, "[]");}
	| direct_declarator '[' ']'     {
                                    $$ = non_terminal(0, "direct_declarator", $1, NULL, NULL, NULL, NULL, "[]");
                                    if($1->expr_type == 1){
                                      $$->expr_type = 1;
                                      $$->symbol = $1->symbol;
                                      $$->nodetype = $1->nodetype+"*";
                                    }
                                      $$->size = get_size($$->nodetype);
                                      $$->expr_type = 15;
                                    }
                                  } 
	| direct_declarator '(' empty1 parameter_type_list ')'  {
                                                            $$ = non_terminal(0, "direct_declarator", $1, $4, NULL, NULL, NULL, "()");
                                                            if($1->expr_type == 1){
                                                              $$->symbol = $1->symbol;
                                                              $$->expr_type = 2;
                                                              $$->nodetype = $1->nodetype;
                                                              if (make_symbol_table($$->symbol)){
                                                                error_throw = true;
                                                                fprintf(stderr, "Error : Redeclaration of function %s", ($$->symbol).c_str());
                                                              }
                                                              else {
                                                                insert_entry($1->symbol, string("func ") + $$->nodetype, 0, 0, false, level, level_id[level]);
                                                                insert_func_args($1->symbol, func_args);
                                                              }
                                                              func_args = "";
                                                              $$->size = get_size($$->nodetype);
                                                            }
                                                          }
  | direct_declarator '(' empty1 identifier_list ')'      {
                                                            $$ = non_terminal(0, "direct_declarator", $1, $4, NULL, NULL, NULL, "()");
                                                            $$->symbol = $$->symbol;
                                                            $$->nodetype = $1->nodetype;
                                                            $$->size = get_size($$->nodetype);
                                                          }
	| direct_declarator '(' empty1 ')'                      {
                                                            $$ = non_terminal(0, "direct_declarator", $1, NULL, NULL, NULL, NULL, "()");
                                                            if($1->expr_type == 1){
                                                              $$->symbol = $1->symbol;
                                                              if (make_symbol_table($$->symbol)){
                                                                error_throw = true;
                                                                fprintf(stderr, "Error : Redeclaration of function %s", ($$->symbol).c_str());
                                                              }
                                                              else {
                                                                insert_entry($1->symbol, string("func ") + $$->nodetype, 0, 0, false, level, level_id[level]);
                                                                insert_func_args($1->symbol, "");
                                                              }                                         
                                                              $$->expr_type = 2;
                                                              func_args = "";
                                                            }
                                                            $$->nodetype = $1->nodetype;
                                                            $$->size = get_size($$->nodetype);
                                                          }
	;

empty1
  : %empty{
          t_name = "";
          func_args = "";
        }
  ;


pointer
  : '*'                             {$$ = terminal($1); $$->nodetype="*";}
  | '*' type_qualifier_list         {$$ = non_terminal(0, $1, $2); $$->nodetype="*";}
  | '*' pointer                     {$$ = non_terminal(0, $1, $2); $$->nodetype="*"+$2->nodetype;}
  | '*' type_qualifier_list pointer {$$ = non_terminal(0, $1, $2, $3); $$->nodetype="*"+$3->nodetype;}
  ;


type_qualifier_list
	: type_qualifier                      {$$=$1;}
	| type_qualifier_list type_qualifier  {$$=non_terminal(0, "type_qualifier_list", $1, $2);}
	;

parameter_type_list
	: parameter_list                {$$ = $1;}
	| parameter_list ',' ELLIPSIS   {func_args += ",..."; $$ = non_terminal(0, "parameter_type_list", $1, terminal($3)); }
	;

parameter_list
	: parameter_declaration                     {$$ = $1; }
	| parameter_list ',' parameter_declaration  {$$ = non_terminal(0, "parameter_list", $1, $3); }
	;

parameter_declaration
	: declaration_specifiers declarator     {
                                            $$ = non_terminal(0, "parameter_declaration", $1, $2);
                                            t_name = "";
                                            if($2->expr_type == 1){
                                              if(lookup($2->symbol, level, level_id[level])){
                                                error_throw = true;
                                                fprintf(stderr, "Error : Redeclaration of %s", $2->symbol);
                                              } 
                                              else insert_entry($2->symbol, $2->nodetype, $2->size, 0, true, level, level_id[level]);
                                              func_args = (func_args == "") ?  $2->nodetype : func_args+","+$2->nodetype;
                                            }
                                          }
	| declaration_specifiers                {
                                            $$ = $1; t_name = "";
                                          }
	;

identifier_list
	: IDENTIFIER                        {$$ = terminal($1); }
	| identifier_list ',' IDENTIFIER    {$$ = non_terminal(0, "identifier_list", $1, terminal($3)); }
	;

type_name
	: specifier_qualifier_list                      {$$ = $1; }
	;

initializer
	: assignment_expression         {$$ = $1; }
	| '{' initializer_list '}'      {$$ = $2; $$->nodetype = $2->nodetype+"*";}
	| '{' initializer_list ',' '}'  {$$ = non_terminal(0, "initializer", $2, NULL, NULL, NULL, NULL, $3); $$->nodetype = $2->nodetype+"*"; $$->expr_type = $2->expr_type; }
	;

initializer_list
	: initializer                       {$$ = $1; $$->expr_type = 1;}
	| initializer_list ',' initializer  {
                                        $$ = non_terminal(0, "initializer_list", $1, $3); 
                                        $$->nodetype = $1->nodetype;
                                        string chk = is_valid($1->nodetype, $3->nodetype);
                                        if(chk == string("0")){
                                          fprintf(stderr, "Warning : Assignment with incompatible pointer types")
                                        }
                                        else if (chk == ""){
                                          error_throw = true;
                                          fprintf(stderr, "Error : Incompatible types when initializing type %s to %s", $1->nodetype, $3->nodetype);
                                        }
                                        $$->expr_type = $1->expr_type+1;
                                      }
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
	: op_brace cl_brace                                   {$$ = terminal("{}");}
	| op_brace statement_list cl_brace                    {$$ = $2; }
	| op_brace declaration_list cl_brace                  {$$ = $2; }
	| op_brace declaration_list statement_list cl_brace   {$$ = non_terminal(0, "compound_statement", $2, $3); }
	;

op_brace
  : '{'       {
                level++;
                level_id[level]++;
              }
  ;

cl_brace
  : '}'       {
                level--;
                if (level == 0){
                  set_current_sym_tab("");
                }
              }

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
	: function_definition   {$$ = $1; t_name="";}
	| declaration           {$$ = $1; t_name="";}
	;

function_definition
	: declaration_specifiers declarator empty2 declaration_list compound_statement  {
                                                                                    $$ = non_terminal(3, "function_definition", $1, $2, $4, $5); 
                                                                                    t_name = "";
                                                                                    sym_tab_entry* entry = lookup(func_name, level, level_id[level]);
                                                                                    if (entry){
                                                                                      entry->init = true;
                                                                                    }
                                                                                  }
	| declaration_specifiers declarator empty2 compound_statement                  {
                                                                                    $$ = non_terminal(3, "function_definition", $1, $2, $4);
                                                                                    t_name = "";
                                                                                    sym_tab_entry* entry = lookup(func_name, level, level_id[level]);
                                                                                    if (entry){
                                                                                      entry->init = true;
                                                                                    }
                                                                                  }
	;

empty2
  : %empty{
          set_current_sym_tab(func_name);
          t_name = "";
        }
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
            yyin = fopen(argv[i+2], "r");
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
    for (int i=0; i<MAX_LEVELS; i++) level_id[i] = 0;
    tab_init();
    graph_init();
    yyparse();
    graph_end();
    if (error_throw){
      // remove ast
      return 0;
    }
    dump_tables();
    fclose (yyin);
    fclose (ast);
    return 0;
}

// enforce stricter checks on expr types
// enum implement
// bool implement
// storage specifiers and type qualifier implement
// incomplete and buggy structs implementation
// buggy scoping
// handle constants