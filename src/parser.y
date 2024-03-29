%{
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "nodes.h"
#include "type_check.h"
#include "codegen.h"

#define MAX_LEVELS 1024
using namespace std;

unordered_set<string> decl_track;
bool error_throw = false;
extern sym_tab* curr;
string func_name;
string func_args;
string func_symbols;
string t_name;
int loop = 0;
stack <qid> switch_expr_stack;
int struct_id = 0;
unsigned long long level_id[MAX_LEVELS];
unsigned long long level = 0;

int yylex(void);
void yyerror(char *s);
FILE *ast;

extern int line;
%}


%union {
  char* str;      /* node label */
  node* ptr;     /* node pointer */
  number* constant; /* constant pointer */
  int addr;
};

%token <str> IDENTIFIER STRING_LITERAL SIZEOF
%token <str> PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token <str> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token <str> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token <str> XOR_ASSIGN OR_ASSIGN
%token <str> TYPEDEF EXTERN STATIC AUTO REGISTER
%token <str> BOOL CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token <str> STRUCT UNION ENUM ELLIPSIS
%token <str> CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN TRUE FALSE
%token <str> '>' '<' '&' '|' '^' '=' '*' ',' ';'
%token <constant> INT_C FLOAT_C CHAR_LITERAL NULL_PTR

%start translation_unit

%type <str> empty1 empty2 empty3 op_brace cl_brace assignment_operator loop_mark
%type <ptr> marker1 marker2 marker3 N switch_case_marker expr_marker exprstmt_marker if_expression switch_expr_marker
%type <ptr> multiplicative_expression additive_expression cast_expression primary_expression expression
%type <ptr> type_name assignment_expression postfix_expression argument_expression_list initializer_list unary_expression
%type <ptr> unary_operator shift_expression equality_expression and_expression exclusive_or_expression inclusive_or_expression
%type <ptr> logical_or_expression logical_and_expression conditional_expression declaration constant_expression declaration_specifiers
%type <ptr> init_declarator_list storage_class_specifier type_specifier type_qualifier
%type <ptr> declarator initializer struct_or_union_specifier enum_specifier struct_or_union struct_declaration_list
%type <ptr> struct_declaration specifier_qualifier_list struct_declarator_list struct_declarator enumerator pointer
%type <ptr> direct_declarator type_qualifier_list parameter_type_list parameter_list parameter_declaration
%type <ptr> labeled_statement compound_statement expression_statement declaration_list
%type <ptr> selection_statement iteration_statement jump_statement external_declaration translation_unit function_definition statement
%type <ptr> relational_expression init_declarator statement_list enumerator_list
%type <addr> M

%%

primary_expression
  : IDENTIFIER                    {$$ = terminal($1);
                                    string type = id_type($1, level, level_id);
                                    if (type != "null"){
                                      $$->init = lookup_use($1, level, level_id)->init;
                                      $$->nodetype = new char[type.size()+1];
                                      strcpy($$->nodetype, type.c_str());
                                      string id($1);
                                      $$->symbol = new char[id.size()+1];
                                      strcpy($$->symbol, id.c_str());
                                      $$->expr_type = 3;

                                      if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                        $$->place = lookup_use($1, level, level_id);

                                        if (is_type_ptr($$->nodetype) && $$->place->is_arr){
                                          qid t = newtmp(type, level, level_id);
                                          emit("&", NULL, $$->place, t);
                                          $$->address = newtmp(type, level, level_id);
                                          emit("&", NULL, t, $$->address);
                                          $$->place = t;
                                        }
                                        else if (is_type_struct($$->nodetype)){
                                          $$->address = newtmp(type + "*", level, level_id);
                                          emit("&", NULL, $$->place, $$->address);
                                        }
                                      }
                                    }
                                    else {
                                      error_throw = true;
                                      $$->nodetype = new char[type.size()+1];
                                      strcpy($$->nodetype, type.c_str());
                                      fprintf(stderr, "%d |\t Error : %s is not declared in this scope\n", line, $1);
                                    }
                                  }
  | INT_C                        {$$ = terminal("INT_C");
                                    $$->init = true;
                                    $$->nodetype = "int";
                                    $$->expr_type = 5;
                                    $$->int_val = $1->int_val;

                                    if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                      qid t = newtmp($$->nodetype, level, level_id);
                                      $$->place = t;
                                      int k = emit("=", NULL, NULL, $$->place);
                                      patch_constant(to_string($$->int_val), k);
                                    }
                                  }
  | FLOAT_C                      {$$ = terminal("FLOAT_C");
                                    $$->init = true;
                                    $$->nodetype = "float";
                                    $$->expr_type = 5;
                                    $$->float_val = $1->float_val;

                                    if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                      qid t = newtmp($$->nodetype, level, level_id);
                                      $$->place = t;
                                      int k = emit("=", NULL, NULL, $$->place);
                                      patch_constant(to_string($$->float_val), k);
                                    }
                                  }
  | TRUE                          {$$ = terminal("TRUE");
                                    $$->nodetype = "bool";
                                    $$->init = true;
                                    $$->bool_val = true;
                                    $$->expr_type = 5;

                                    if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                      qid t = newtmp($$->nodetype, level, level_id);
                                      $$->place = t;
                                      int k = emit("=", NULL, NULL, $$->place);
                                      patch_constant(to_string(1), k);
                                    }
                                  }
  | FALSE                         {$$ = terminal("FALSE");
                                    $$->nodetype = "bool";
                                    $$->init = true;
                                    $$->bool_val = false;
                                    $$->expr_type = 5;

                                    if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                      qid t = newtmp($$->nodetype, level, level_id);
                                      $$->place = t;
                                      int k = emit("=", NULL, NULL, $$->place);
                                      patch_constant(to_string(0), k);
                                    }
                                  }
  | STRING_LITERAL                {$$ = terminal("STRING_LITERAL");
                                    $$->nodetype = "char*";
                                    $$->init = true;
                                    $$->expr_type = 5;
                                    string s($1);
                                    if (s.size() == 2){
                                      error_throw = true;
                                      fprintf(stderr, "%d |\t Error : Cannot initialize an empty string literal\n", line);
                                    }
                                    s = s.substr(1, s.size()-2);
                                    int size = get_real_size(s);

                                    if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                      qid t = newstring($$->nodetype, size+1, level, level_id);
                                      set_arr_flag(t->sym_name, level, level_id[level]);
                                      $$->place = newtmp($$->nodetype, level, level_id);
                                      emit("&", NULL, t, $$->place);
                                      int k = emit("=string", NULL, NULL, t);
                                      patch_constant(s, k);
                                      $$->address = newtmp($$->nodetype, level, level_id);
                                      emit("&", NULL, $$->place, $$->address);
                                    }
                                  }
  | CHAR_LITERAL                  {$$ = terminal("CHAR_LITERAL");
                                    $$->nodetype = "char";
                                    $$->init = true;
                                    $$->int_val = $1->int_val;
                                    $$->expr_type = 5;

                                    if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                      qid t = newtmp($$->nodetype, level, level_id);
                                      $$->place = t;
                                      int k = emit("=", NULL, NULL, $$->place);
                                      patch_constant(to_string($$->int_val), k);
                                    }
                                  }
  | NULL_PTR                      {$$ = terminal("NULL");
                                    $$->init = true;
                                    $$->nodetype = "void*";
                                    $$->expr_type = 5;
                                    $$->int_val = 0;

                                    if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                      qid t = newtmp($$->nodetype, level, level_id);
                                      $$->place = t;
                                      int k = emit("=", NULL, NULL, $$->place);
                                      patch_constant(to_string(0), k);
                                    }
                                  }
  | '(' expression ')'            {$$ = $2; $$->symbol = "symbol"; }
  ;

postfix_expression
  : primary_expression                       {$$ = $1; }
  | postfix_expression '[' inclusive_or_expression ']'    {$$ = non_terminal(0, "postfix_expression[expression]", $1, $3);
                                                            if ($1->init && $3->init){
                                                              $$->init = true;
                                                            }
                                                            $$->symbol = $1->symbol;
                                                            string type = postfix_type($1->nodetype, $3->nodetype, 1);
                                                            if (type != "null"){
                                                              $$->nodetype = new char[type.size()+1];
                                                              strcpy($$->nodetype, type.c_str());
                                                              if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                qid off = newtmp("int", level, level_id);
                                                                int k = emit("*int", $3->place, NULL, off);
                                                                patch_constant(to_string(get_size($$->nodetype, level, level_id)), k);
                                                                $$->address = newtmp($1->nodetype, level, level_id);
                                                                emit("+ptr", $1->place, off, $$->address);
                                                                $$->place = newtmp($$->nodetype, level, level_id);
                                                                if (!is_type_struct($$->nodetype)) emit("*", NULL, $$->address, $$->place);
                                                              }
                                                            }
                                                            else {
                                                              error_throw = true;
                                                              $$->nodetype = new char[type.size()+1];
                                                              strcpy($$->nodetype, type.c_str());
                                                              fprintf(stderr, "%d |\t Error : Array \"%s\" indexed with more indices than its dimension or array index is not an integer\n", line, ($1->symbol));
                                                            }
                                                          }
  | postfix_expression '(' ')'               {qid tmp = $1->place;
                                              $$ = $1;
                                              $$->init = true;
                                              string type = postfix_type($1->nodetype, "", 2);
                                              $$->nodetype = new char[type.size()+1];
                                              strcpy($$->nodetype, type.c_str());
                                              if (type != "null"){
                                                if ($1->expr_type == 3){
                                                  string args = get_func_args($1->symbol);
                                                  if (args != "null"){
                                                    error_throw = true;
                                                    fprintf(stderr, "%d |\t Error : %s function requires some arguments to be passed\n", line, ($1->symbol));
                                                  }
                                                }
                                                if(!error_throw){
                                                  if (type == "void"){
                                                    qid t = NULL;
                                                    $$->place = t;
                                                  }
                                                  else {
                                                    qid t = newtmp($$->nodetype, level, level_id);
                                                    $$->place = t;
                                                  }
                                                  emit("params", NULL, NULL, NULL);
                                                  emit("call", NULL, tmp, $$->place);
                                                }
                                              }
                                              else {
                                                error_throw = true;
                                                fprintf(stderr, "%d |\t Error : Invalid function call for the function \"%s\"\n", line, ($1->symbol));
                                              }
                                             }
  | postfix_expression '(' argument_expression_list ')'   {$$ = non_terminal(0, "postfix_expression(argument_expression_list)", $1, $3);
                                                            if ($3->init) $$->init = true;
                                                            $$->symbol = $1->symbol;
                                                            string type = postfix_type($1->nodetype, "", 3);
                                                            $$->nodetype = new char[type.size()+1];
                                                            strcpy($$->nodetype, type.c_str());
                                                            if (type != "null"){
                                                              if ($1->expr_type == 3){
                                                                string args = get_func_args($1->symbol);
                                                                string temp1($3->curr_args_types);
                                                                string temp2 = args == "null" ? "" : args;
                                                                string typeA, typeB;
                                                                string delim = string(",");
                                                                size_t f1 = 1;
                                                                size_t f2 = 1;
                                                                int arg_num = 0;
                                                                while(f1 != string::npos && f2 != string::npos){
                                                                  f1 = temp1.find_first_of(delim);
                                                                  f2 = temp2.find_first_of(delim);
                                                                  arg_num++;
                                                                  if (f1 == string::npos) typeA = temp1;
                                                                  else {
                                                                    typeA = temp1.substr(0,f1);
                                                                    temp1 = temp1.substr(f1+1);
                                                                  }
                                                                  if (f2 == string::npos) typeB = temp2;
                                                                  else {
                                                                    typeB = temp2.substr(0,f2);
                                                                    temp2 = temp2.substr(f2+1);
                                                                  }
                                                                  string chk = is_valid(typeA, typeB);
                                                                  if (chk == "0"){
                                                                    fprintf(stderr, "%d |\t Warning : Passing argument %d of %s from incompatible pointer type. Note : expected %s but argument is of type %s\n", line,  arg_num, ($1->symbol), typeB.c_str(), typeA.c_str());
                                                                  }
                                                                  else if (chk == "null"){
                                                                    error_throw = true;
                                                                    fprintf(stderr, "%d |\t Error : Incompatible type for argument %d of %s. Note : expected %s but argument is of type %s\n", line, arg_num, ($1->symbol), typeB.c_str(), typeA.c_str());
                                                                  }
                                                                  else if (chk == "1"){
                                                                    if ((is_type_float(typeA) &&  !is_type_float(typeB)) || (!is_type_float(typeA) &&  is_type_float(typeB))){
                                                                      error_throw = true;
                                                                      fprintf(stderr, "%d | \t Error : Implicit typecasting of float to non-float or non-float to float is not allowed in function arguments. Check argument number %d of function call %s\n", line, arg_num, $1->symbol);
                                                                    }
                                                                  }
                                                                  if ((f1 != string::npos) && (f2 != string::npos)){
                                                                    continue;
                                                                  }
                                                                  else if (f2 != string::npos){
                                                                    error_throw = true;
                                                                    fprintf(stderr, "%d |\t Error : Too few arguments for the function %s\n", line, ($1->symbol));
                                                                    break;
                                                                  }
                                                                  else if (f1 != string::npos){
                                                                    error_throw = true;
                                                                    fprintf(stderr, "%d |\t Error : Too many arguments for the function %s\n", line, ($1->symbol));
                                                                    break;
                                                                  }
                                                                  else break;
                                                                }
                                                              }
                                                              if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                if (type == "void");
                                                                else $$->place = newtmp($$->nodetype, level, level_id);
                                                                int k = emit("call", NULL, $1->place, $$->place);
                                                              }
                                                            }
                                                            else {
                                                              error_throw = true;
                                                              fprintf(stderr, "%d |\t Error : Invalid function call %s\n", line, ($1->symbol));
                                                            }
                                                          }
  | postfix_expression '.' IDENTIFIER        {$$ = non_terminal(0, "postfix_expression.IDENTIFIER", $1, terminal($3));
                                              string type = postfix_type($1->nodetype, "", 4);
                                              if (type == "null"){
                                                $$->nodetype = new char[type.size()+1];
                                                strcpy($$->nodetype, type.c_str());
                                                error_throw = true;
                                                fprintf(stderr, "%d |\t Error : \'%s\' is either a struct pointer or not a valid struct\n", line, $1->symbol);
                                              }
                                              else {
                                                string type = struct_sym_type($1->nodetype, $3, level, level_id);
                                                if (type == "null"){
                                                  $$->nodetype = new char[type.size()+1];
                                                  strcpy($$->nodetype, type.c_str());
                                                  error_throw = true;
                                                  fprintf(stderr, "%d |\t Error : \'%s\' is not a member of the struct being used\n", line, $3);
                                                }
                                                else {
                                                  $$->nodetype = new char[type.size()+1];
                                                  strcpy($$->nodetype, type.c_str());
                                                  string symbol = string($1->symbol) + "." + string($3);
                                                  $$->symbol = new char[symbol.size()+1];
                                                  strcpy($$->symbol, symbol.c_str());

                                                  if (!error_throw){
                                                    $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                    long long offset = get_struct_sym_offset($1->nodetype, $3, level, level_id);
                                                    string id_type($$->nodetype);
                                                    if (is_type_ptr(id_type)){
                                                      if (is_struct_sym_arr($1->nodetype, $3, level, level_id)){
                                                        $$->place = newtmp(id_type, level, level_id);
                                                        int k = emit("+ptr", $1->address, NULL, $$->place);
                                                        patch_constant(to_string(offset), k);
                                                        $$->address = newtmp(id_type, level, level_id);
                                                        emit("&", NULL, $$->place, $$->address);
                                                      }
                                                      else {
                                                        $$->address = newtmp(id_type, level, level_id);
                                                        int k = emit("+ptr", $1->address, NULL, $$->address);
                                                        patch_constant(to_string(offset), k);
                                                        $$->place = newtmp(id_type, level, level_id);
                                                        emit("*", NULL, $$->address, $$->place);
                                                      }
                                                    }
                                                    else {
                                                      $$->address = newtmp(id_type + "*", level, level_id);
                                                      int k = emit("+ptr", $1->address, NULL, $$->address);
                                                      patch_constant(to_string(offset), k);
                                                      $$->place = newtmp($$->nodetype, level, level_id);
                                                      if (!is_type_struct($$->nodetype)) emit("*", NULL, $$->address, $$->place);
                                                      else $$->place->size = 0;
                                                    }
                                                  }
                                                }
                                              }
                                             }
  | postfix_expression PTR_OP IDENTIFIER     {$$ = non_terminal(0, $2, $1, terminal($3));
                                              string type = postfix_type($1->nodetype, "", 5);
                                              if (type == "null"){
                                                $$->nodetype = new char[type.size()+1];
                                                strcpy($$->nodetype, type.c_str());
                                                error_throw = true;
                                                fprintf(stderr, "%d |\t Error : \'%s\' is either a struct multi pointer or not a valid struct\n", line, $1->symbol);
                                              }
                                              else {
                                                string tmp($1->nodetype); tmp.pop_back();
                                                string type = struct_sym_type(tmp, $3, level, level_id);
                                                if (type == "null"){
                                                  $$->nodetype = new char[type.size()+1];
                                                  strcpy($$->nodetype, type.c_str());
                                                  error_throw = true;
                                                  fprintf(stderr, "%d |\t Error : \'%s\' is not a member of the struct being used\n", line, $3);
                                                }
                                                else {
                                                  $$->nodetype = new char[type.size()+1];
                                                  strcpy($$->nodetype, type.c_str());
                                                  string symbol = string($1->symbol) + "->" + string($3);
                                                  $$->symbol = new char[symbol.size()+1];
                                                  strcpy($$->symbol, symbol.c_str());

                                                  if (!error_throw){
                                                    $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                    long long offset = get_struct_sym_offset(tmp, $3, level, level_id);
                                                    string id_type($$->nodetype);
                                                    if (is_type_ptr(id_type)){
                                                      if (is_struct_sym_arr(tmp, $3, level, level_id)){
                                                        $$->place = newtmp(id_type, level, level_id);
                                                        int k = emit("+ptr", $1->place, NULL, $$->place);
                                                        patch_constant(to_string(offset), k);
                                                        $$->address = newtmp(id_type, level, level_id);
                                                        emit("&", NULL, $$->place, $$->address);
                                                      }
                                                      else {
                                                        $$->address = newtmp(id_type, level, level_id);
                                                        int k = emit("+ptr", $1->place, NULL, $$->address);
                                                        patch_constant(to_string(offset), k);
                                                        $$->place = newtmp(id_type, level, level_id);
                                                        emit("*", NULL, $$->address, $$->place);
                                                      }
                                                    }
                                                    else {
                                                      $$->address = newtmp(id_type + "*", level, level_id);
                                                      int k = emit("+ptr", $1->place, NULL, $$->address);
                                                      patch_constant(to_string(offset), k);
                                                      $$->place = newtmp($$->nodetype, level, level_id);
                                                      if (!is_type_struct($$->nodetype)) emit("*", NULL, $$->address, $$->place);
                                                      else $$->place->size = 0;
                                                    }
                                                  }
                                                }
                                              }
                                             }
  | postfix_expression INC_OP                {$$ = non_terminal(0, $2, $1);
                                              if ($1->init) $$->init = true;
                                              string type = postfix_type($1->nodetype, "", 6);
                                              $$->nodetype = new char[type.size()+1];
                                              strcpy($$->nodetype, type.c_str());
                                              $$->symbol = $1->symbol;
                                              if (type == "null"){
                                                error_throw = true;
                                                fprintf(stderr, "%d |\t Error : Increment used with incompatible type. Allowed types are int, char, float\n", line);
                                              }
                                              if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                if ($1->address){
                                                  $$->address = $1->address;
                                                  $$->place = newtmp($$->nodetype, level, level_id);
                                                  emit("=", NULL, $1->place, $$->place);
                                                  if (is_type_float($$->nodetype)){
                                                    int k = emit("+float", $1->place, NULL, $1->place);
                                                    patch_constant(to_string(1.0), k);
                                                    emit("* =", NULL, $1->place, $$->address);
                                                  }
                                                  else {
                                                    int k = emit("+int", $1->place, NULL, $1->place);
                                                    patch_constant(to_string(1), k);
                                                    emit("* =", NULL, $1->place, $$->address);
                                                  }
                                                }
                                                else {
                                                  $$->place = newtmp($$->nodetype, level, level_id);
                                                  emit("=", NULL, $1->place, $$->place);
                                                  if (is_type_float($$->nodetype)){
                                                    int k = emit("+float", $1->place, NULL, $1->place);
                                                    patch_constant(to_string(1.0), k);
                                                  }
                                                  else {
                                                    int k = emit("+int", $1->place, NULL, $1->place);
                                                    patch_constant(to_string(1), k);
                                                  }
                                                }
                                              }
                                             }
  | postfix_expression DEC_OP                {$$ = non_terminal(0, $2, $1);
                                              if ($1->init) $$->init = true;
                                              string type = postfix_type($1->nodetype, "", 7);
                                              $$->nodetype = new char[type.size()+1];
                                              strcpy($$->nodetype, type.c_str());
                                              $$->symbol = $1->symbol;
                                              if (type == "null"){
                                                error_throw = true;
                                                fprintf(stderr, "%d |\t Error : Decrement used with incompatible type. Allowed types are int, char, float\n", line);
                                              }
                                              if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                if ($1->address){
                                                  $$->address = $1->address;
                                                  $$->place = newtmp($$->nodetype, level, level_id);
                                                  emit("=", NULL, $1->place, $$->place);
                                                  if (is_type_float($$->nodetype)){
                                                    int k = emit("-float", $1->place, NULL, $1->place);
                                                    patch_constant(to_string(1.0), k);
                                                    emit("* =", NULL, $1->place, $$->address);
                                                  }
                                                  else {
                                                    int k = emit("-int", $1->place, NULL, $1->place);
                                                    patch_constant(to_string(1), k);
                                                    emit("* =", NULL, $1->place, $$->address);
                                                  }
                                                }
                                                else {
                                                  $$->place = newtmp($$->nodetype, level, level_id);
                                                  emit("=", NULL, $1->place, $$->place);
                                                  if (is_type_float($$->nodetype)){
                                                    int k = emit("-float", $1->place, NULL, $1->place);
                                                    patch_constant(to_string(1.0), k);
                                                  }
                                                  else {
                                                    int k = emit("-int", $1->place, NULL, $1->place);
                                                    patch_constant(to_string(1), k);
                                                  }
                                                }
                                              }
                                             }
  ;

argument_expression_list
  : assignment_expression            {$$ = $1;
                                      string temp($$->nodetype);
                                      $$->curr_args_types = new char[temp.size()+1];
                                      strcpy($$->curr_args_types, temp.c_str());
                                      if (!($$->nodetype == "null")) $$->nodetype = "void";
                                      if(!error_throw){
                                        int tmp = emit("params", NULL, $$->place, NULL);
                                        backpatch($$->nextlist, tmp);
                                      }
                                     }
  | argument_expression_list ',' assignment_expression    {$$ = non_terminal(0, "argument_expression_list", $1, $3);
                                                            string type = args_type($1->nodetype, $3->nodetype);
                                                            $$->nodetype = new char[type.size()+1];
                                                            strcpy($$->nodetype, type.c_str());
                                                            if ($1->init && $3->init) $$->init = true;
                                                            string append = string($1->curr_args_types) + "," + string($3->nodetype);
                                                            $$->curr_args_types = new char[append.size()+1];
                                                            strcpy($$->curr_args_types, append.c_str());
                                                            if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                              int tmp = emit("params", NULL, $3->place, NULL);
                                                              backpatch($3->nextlist, tmp);
                                                            }
                                                          }
  ;

unary_expression
  : postfix_expression            {$$ = $1; }
  | INC_OP unary_expression       {$$ = non_terminal(0, $1, $2);
                                    if ($2->init) $$->init = true;
                                    string type = postfix_type($2->nodetype, "", 6);
                                    $$->nodetype = new char[type.size()+1];
                                    strcpy($$->nodetype, type.c_str());
                                    $$->symbol = $2->symbol;
                                    if (type == "null"){
                                      error_throw = true;
                                      fprintf(stderr, "%d |\t Error : Increment used with incompatible type. Allowed types are int, char, float\n", line);
                                    }
                                    if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                      if ($2->address){
                                        $$->address = $2->address;
                                        $$->place = $2->place;
                                        if (is_type_float($$->nodetype)){
                                          int k = emit("+float", $$->place, NULL, $$->place);
                                          patch_constant(to_string(1.0), k);
                                          emit("* =", NULL, $2->place, $$->address);
                                        }
                                        else {
                                          int k = emit("+int", $$->place, NULL, $$->place);
                                          patch_constant(to_string(1), k);
                                          emit("* =", NULL, $2->place, $$->address);
                                        }
                                      }
                                      else {
                                        $$->place = $2->place;
                                        if (is_type_float($$->nodetype)){
                                          int k = emit("+float", $$->place, NULL, $$->place);
                                          patch_constant(to_string(1.0), k);
                                        }
                                        else {
                                          int k = emit("+int", $$->place, NULL, $$->place);
                                          patch_constant(to_string(1), k);
                                        }
                                      }
                                    }
                                  }

  | DEC_OP unary_expression       {$$ = non_terminal(0, $1, $2);
                                    if ($2->init) $$->init = true;
                                    string type = postfix_type($2->nodetype, "", 7);
                                    $$->nodetype = new char[type.size()+1];
                                    strcpy($$->nodetype, type.c_str());
                                    $$->symbol = $2->symbol;
                                    if (type == "null"){
                                      error_throw = true;
                                      fprintf(stderr, "%d |\t Error : Decrement used with incompatible type. Allowed types are int, char, float\n", line);
                                    }
                                    if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                      if ($2->address){
                                        $$->address = $2->address;
                                        $$->place = $2->place;
                                        if (is_type_float($$->nodetype)){
                                          int k = emit("-float", $$->place, NULL, $$->place);
                                          patch_constant(to_string(1.0), k);
                                          emit("* =", NULL, $2->place, $$->address);
                                        }
                                        else {
                                          int k = emit("-int", $$->place, NULL, $$->place);
                                          patch_constant(to_string(1), k);
                                          emit("* =", NULL, $2->place, $$->address);
                                        }
                                      }
                                      else {
                                        $$->place = $2->place;
                                        if (is_type_float($$->nodetype)){
                                          int k = emit("-float", $$->place, NULL, $$->place);
                                          patch_constant(to_string(1.0), k);
                                        }
                                        else {
                                          int k = emit("-int", $$->place, NULL, $$->place);
                                          patch_constant(to_string(1), k);
                                        }
                                      }
                                    }
                                  }
  | unary_operator cast_expression{$$ = non_terminal(0, "unary_expression", $1, $2);
                                    if ($2->init) $$->init = true;
                                    string type = unary_type($1->label, $2->nodetype);
                                    $$->nodetype = new char[type.size()+1];
                                    strcpy($$->nodetype, type.c_str());
                                    $$->symbol = $2->symbol;
                                    string op($1->label);
                                    if (type == "null"){
                                      error_throw = true;
                                      if(op == "&") fprintf(stderr, "%d |\t Error : Trying to use %s with a pointer type. Note : Multi-level-pointers are not allowed\n", line, $1->label);
                                      else if(op == "*") fprintf(stderr, "%d |\t Error : Type inconsistent with %s operator either the symbol is a not a pointer or is a multi-level pointer\n", line, $1->label);
                                      else if(op == "+" || op == "-") fprintf(stderr, "%d |\t Error : Type inconsistent with %s operator. Note : Allowed types are int, float, bool and char\n", line, $1->label);
                                      else if(op == "~") fprintf(stderr, "%d |\t Error : Type inconsistent with %s operator. Note : Allowed types are int, bool and char\n", line, $1->label);
                                      else if(op == "!") fprintf(stderr, "%d |\t Error : Type inconsistent with %s operator. Note : Allowed types are int, float, bool and char\n", line, $1->label);
                                    }
                                    if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                      if (op == "&"){
                                        $$->place = newtmp(type, level, level_id);
                                        if ($2->address){
                                          emit("=", NULL, $2->address, $$->place);
                                          $$->address = newtmp(type, level, level_id);
                                        }
                                        else {
                                          emit("&", NULL, $2->place, $$->place);
                                        }
                                      }
                                      else if (op == "*"){
                                        $$->place = newtmp(type, level, level_id);
                                        $$->address = newtmp($2->nodetype, level, level_id);
                                        emit("=", NULL, $2->place, $$->address);
                                        if (!is_type_struct(type)) emit("*", NULL, $2->place, $$->place);
                                        else $$->place->size = 0;
                                      }
                                      else if (op == "+"){
                                        $$->place = $2->place;
                                      }
                                      else if (op == "-"){
                                        $$->place = newtmp(type, level, level_id);
                                        qid t = emit_assignment(type, $2->nodetype, $2->place, level, level_id, line);
                                        emit($1->label, NULL, t, $$->place);
                                      }
                                      else {
                                        $$->place = newtmp($2->nodetype, level, level_id);
                                        emit($1->label, NULL, $2->place, $$->place);
                                      }
                                    }
                                  }
  | SIZEOF '(' type_name ')'      {$$ = non_terminal(0, $1, $3);
                                    string type = $3->nodetype;
                                    if (!is_valid_type(type, level, level_id)){
                                      error_throw = true;
                                      fprintf(stderr, "%d |\t Error : Invalid type used for typecasting\n", line);
                                    }
                                    else if (type == "void"){
                                      error_throw = true;
                                      fprintf(stderr, "%d |\t Error : Cannot typecast into void type\n", line);
                                    }
                                    if ($3->init) $$->init = true;
                                    $$->nodetype = "int";
                                    if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                      qid t = newtmp($$->nodetype, level, level_id);
                                      $$->place = t;
                                      int k = emit("SIZEOF", NULL, NULL, $$->place);
                                      patch_constant(to_string(get_size($3->nodetype, level, level_id)), k);
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
                                        $$->symbol = $4->symbol;
                                        string tmp($2->nodetype);
                                        if (!is_valid_type($2->nodetype, level, level_id)){
                                          error_throw = true;
                                          fprintf(stderr, "%d |\t Error : Invalid type used for typecasting\n", line);
                                        }
                                        else if (tmp == "void"){
                                          error_throw = true;
                                          fprintf(stderr, "%d |\t Error : Cannot typecast into void type\n", line);
                                        }
                                        string type = cast_type($2->nodetype, $4->nodetype);
                                        $$->nodetype = new char[type.size()+1];
                                        strcpy($$->nodetype, type.c_str());

                                        if (type == "0"){
                                          $$->nodetype = $2->nodetype;
                                          fprintf(stderr, "%d |\t Warning : Incompatible pointer type-casting\n", line);

                                          if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                            qid tmp = emit_assignment($$->nodetype, $4->nodetype, $4->place, level, level_id, line);
                                            $$->place = tmp;
                                          }
                                        }
                                        else if (type == "1"){
                                          $$->nodetype = $2->nodetype;

                                          if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                            qid tmp = emit_assignment($$->nodetype, $4->nodetype, $4->place, level, level_id, line);
                                            $$->place = tmp;
                                          }
                                        }
                                        else {
                                          error_throw = true;
                                          fprintf(stderr, "%d |\t Error : Cannot typecast given expression to %s\n", line, ($2->nodetype));
                                        }
                                        if ($4->init) $$->init = true;
                                      }
  ;

multiplicative_expression
  : cast_expression                                     {$$ = $1; }
  | multiplicative_expression '*' cast_expression       { if ($1->init && $3->init) $$->init = true;
                                                          string type = mul_type($1->nodetype, $3->nodetype, '*');
                                                          if (type != "null"){
                                                            if (type == "int"){
                                                              $$ = non_terminal(0, "* int", $1, $3);
                                                              $$->nodetype = "int";

                                                              if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                qid p1 = emit_assignment($$->nodetype, $1->nodetype, $1->place, level, level_id, line);
                                                                qid p2 = emit_assignment($$->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                                                qid t = newtmp($$->nodetype, level, level_id);
                                                                $$->place = t;
                                                                emit("*int", p1, p2, $$->place);
                                                              }
                                                            }
                                                            else if (type == "float"){
                                                              $$ = non_terminal(0, "* float", $1, $3);
                                                              $$->nodetype = "float";

                                                              if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                qid p1 = emit_assignment($$->nodetype, $1->nodetype, $1->place, level, level_id, line);
                                                                qid p2 = emit_assignment($$->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                                                qid t = newtmp($$->nodetype, level, level_id);
                                                                $$->place = t;
                                                                emit("*float", p1, p2, $$->place);
                                                              }
                                                            }
                                                          }
                                                          else {
                                                            error_throw = true;
                                                            fprintf(stderr, "%d |\t Error : Incompatible type for * operator. Note : Supported operands are int, char, bool and float\n", line);
                                                            $$ = non_terminal(0, "*", $1, $3);
                                                            $$->nodetype = "null";
                                                          }
                                                        }
  | multiplicative_expression '/' cast_expression       {if ($1->init && $3->init) $$->init = true;
                                                          string type = mul_type($1->nodetype, $3->nodetype, '/');
                                                          if (type != "null"){
                                                            if (type == "int"){
                                                              $$ = non_terminal(0, "/ int", $1, $3);
                                                              $$->nodetype = "int";

                                                              if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                qid p1 = emit_assignment($$->nodetype, $1->nodetype, $1->place, level, level_id, line);
                                                                qid p2 = emit_assignment($$->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                                                qid t = newtmp($$->nodetype, level, level_id);
                                                                $$->place = t;
                                                                emit("/int", p1, p2, $$->place);
                                                              }
                                                            }
                                                            else if (type == "float"){
                                                              $$ = non_terminal(0, "/ float", $1, $3);
                                                              $$->nodetype = "float";

                                                              if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                qid p1 = emit_assignment($$->nodetype, $1->nodetype, $1->place, level, level_id, line);
                                                                qid p2 = emit_assignment($$->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                                                qid t = newtmp($$->nodetype, level, level_id);
                                                                $$->place = t;
                                                                emit("/float", p1, p2, $$->place);
                                                              }

                                                            }
                                                          }
                                                          else {
                                                            error_throw = true;
                                                            fprintf(stderr, "%d |\t Error : Incompatible type for / operator. Note : Supported operands are int, char, bool and float\n", line);
                                                            $$ = non_terminal(0, "/", $1, $3);
                                                            $$->nodetype = "null";
                                                          }
                                                        }
  | multiplicative_expression '%' cast_expression       {if ($1->init && $3->init) $$->init = true;
                                                          string type = mul_type($1->nodetype, $3->nodetype, '%');
                                                          if (type != "null"){
                                                            $$ = non_terminal(0, "% int", $1, $3);
                                                            $$->nodetype = "int";

                                                            if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                              qid p1 = emit_assignment($$->nodetype, $1->nodetype, $1->place, level, level_id, line);
                                                              qid p2 = emit_assignment($$->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                                              qid t = newtmp($$->nodetype, level, level_id);
                                                              $$->place = t;
                                                              emit("%int", p1, p2, $$->place);
                                                            }
                                                          }
                                                          else {
                                                            error_throw = true;
                                                            fprintf(stderr, "%d |\t Error : Incompatible type for % operator. Note : Supported operands are int, char and bool\n", line);
                                                            $$ = non_terminal(0, "%", $1, $3);
                                                            $$->nodetype = "null";
                                                          }
                                                        }
  ;

additive_expression
  : multiplicative_expression                           {$$ = $1; }
  | additive_expression '+' multiplicative_expression   {if ($1->init && $3->init) $$->init = true;
                                                          string type = add_type($1->nodetype, $3->nodetype);
                                                          if (type != "null"){
                                                            if (type == "int"){
                                                              char* label; string tmp = "+ " + type; label = &tmp[0];
                                                              $$ = non_terminal(0, label, $1, $3);
                                                              $$->nodetype = "int";

                                                              if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                qid p1 = emit_assignment($$->nodetype, $1->nodetype, $1->place, level, level_id, line);
                                                                qid p2 = emit_assignment($$->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                                                qid t = newtmp($$->nodetype, level, level_id);
                                                                $$->place = t;
                                                                emit("+int", p1, p2, $$->place);
                                                              }
                                                            }
                                                            else if (type == "float"){
                                                              char* label; string tmp = "+ " + type; label = &tmp[0];
                                                              $$ = non_terminal(0, label, $1, $3);
                                                              $$->nodetype = "float";

                                                              if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                qid p1 = emit_assignment($$->nodetype, $1->nodetype, $1->place, level, level_id, line);
                                                                qid p2 = emit_assignment($$->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                                                qid t = newtmp($$->nodetype, level, level_id);
                                                                $$->place = t;
                                                                emit("+float", p1, p2, $$->place);
                                                              }
                                                            }
                                                            else {
                                                              char* label; string tmp = "+ " + type; label = &tmp[0];
                                                              $$ = non_terminal(0, label, $1, $3);
                                                              $$->nodetype = new char[type.size()+1];
                                                              strcpy($$->nodetype, type.c_str());

                                                              if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                if (is_type_int($1->nodetype)){
                                                                  qid t = newtmp($$->nodetype, level, level_id);
                                                                  $$->place = t;
                                                                  qid tmp = newtmp($1->nodetype, level, level_id);
                                                                  string tp($3->nodetype); tp.pop_back();
                                                                  int k = emit("*int", $1->place, NULL, tmp);
                                                                  patch_constant(to_string(get_size(tp, level, level_id)), k);
                                                                  emit("+ptr", tmp, $3->place, $$->place);
                                                                }
                                                                else {
                                                                  qid t = newtmp($$->nodetype, level, level_id);
                                                                  $$->place = t;
                                                                  qid tmp = newtmp($3->nodetype, level, level_id);
                                                                  string tp($1->nodetype); tp.pop_back();
                                                                  int k = emit("*int", $3->place, NULL, tmp);
                                                                  patch_constant(to_string(get_size(tp, level, level_id)), k);
                                                                  emit("+ptr", $1->place, tmp, $$->place);
                                                                }
                                                              }
                                                            }
                                                          }
                                                          else {
                                                            error_throw = true;
                                                            fprintf(stderr, "%d |\t Error : Incompatible type for + operator. Note : Each operand should of type int, char, bool or float and if one operand is a pointer then the other should be an integer\n", line);
                                                            $$ = non_terminal(0, "+", $1, $3);
                                                            $$->nodetype = "null";
                                                          }
                                                        }
  | additive_expression '-' multiplicative_expression   {if ($1->init && $3->init) $$->init = true;
                                                          string type = add_type($1->nodetype, $3->nodetype);
                                                          if (type != "null"){
                                                            if (type == "int"){
                                                              char* label; string tmp = "- " + type; label = &tmp[0];
                                                              $$ = non_terminal(0, label, $1, $3);
                                                              $$->nodetype = "int";

                                                              if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                qid p1 = emit_assignment($$->nodetype, $1->nodetype, $1->place, level, level_id, line);
                                                                qid p2 = emit_assignment($$->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                                                qid t = newtmp($$->nodetype, level, level_id);
                                                                $$->place = t;
                                                                emit("-int", p1, p2, $$->place);
                                                              }
                                                            }
                                                            else if (type == "float"){
                                                              char* label; string tmp = "- " + type; label = &tmp[0];
                                                              $$ = non_terminal(0, label, $1, $3);
                                                              $$->nodetype = "float";

                                                              if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                qid p1 = emit_assignment($$->nodetype, $1->nodetype, $1->place, level, level_id, line);
                                                                qid p2 = emit_assignment($$->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                                                qid t = newtmp($$->nodetype, level, level_id);
                                                                $$->place = t;
                                                                emit("-float", p1, p2, $$->place);
                                                              }
                                                            }
                                                            else {
                                                              char* label; string tmp = "- " + type; label = &tmp[0];
                                                              $$ = non_terminal(0, label, $1, $3);
                                                              $$->nodetype = new char[type.size()+1];
                                                              strcpy($$->nodetype, type.c_str());

                                                              if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                if (is_type_int($1->nodetype)){
                                                                  qid t = newtmp($$->nodetype, level, level_id);
                                                                  $$->place = t;
                                                                  qid tmp = newtmp($1->nodetype, level, level_id);
                                                                  string tp($3->nodetype); tp[tp.size()-1] = '\0';
                                                                  int k = emit("*int", $1->place, NULL, tmp);
                                                                  patch_constant(to_string(get_size(tp, level, level_id)), k);
                                                                  emit("-ptr", tmp, $3->place, $$->place);
                                                                }
                                                                else {
                                                                  qid t = newtmp($$->nodetype, level, level_id);
                                                                  $$->place = t;
                                                                  qid tmp = newtmp($3->nodetype, level, level_id);
                                                                  string tp($1->nodetype); tp[tp.size()-1] = '\0';
                                                                  int k = emit("*int", $3->place, NULL, tmp);
                                                                  patch_constant(to_string(get_size(tp, level, level_id)), k);
                                                                  emit("-ptr", $1->place, tmp, $$->place);
                                                                }
                                                              }
                                                            }
                                                          }
                                                          else {
                                                            error_throw = true;
                                                            fprintf(stderr, "%d |\t Error : Incompatible type for - operator. Note : Each operand should of type int, char, bool or float and if one operand is a pointer then the other should be an integer\n", line);
                                                            $$ = non_terminal(0, "-", $1, $3);
                                                            $$->nodetype = "null";
                                                          }
                                                        }
  ;


shift_expression
  : additive_expression                           {$$ = $1; }
  | shift_expression LEFT_OP additive_expression  {$$ = non_terminal(0, $2, $1, $3);
                                                    if ($1->init && $3->init) $$->init = true;
                                                    string type = shift_type($1->nodetype, $3->nodetype);
                                                    $$->nodetype = new char[type.size()+1];
                                                    strcpy($$->nodetype, type.c_str());
                                                    if (type == "null"){
                                                      error_throw = true;
                                                      fprintf(stderr, "%d |\t Error : Invalid operand(s) with <<. Note : Each operand should of type int or char\n", line);
                                                    }
                                                    else {
                                                      if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                        qid p1 = emit_assignment($$->nodetype, $1->nodetype, $1->place, level, level_id, line);
                                                        qid p2 = emit_assignment($$->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                                        qid t = newtmp($$->nodetype, level, level_id);
                                                        $$->place = t;
                                                        emit("<<", p1, p2, $$->place);
                                                      }
                                                    }
                                                  }
  | shift_expression RIGHT_OP additive_expression {$$ = non_terminal(0, $2, $1, $3);
                                                    if ($1->init && $3->init) $$->init = true;
                                                    string type = shift_type($1->nodetype, $3->nodetype);
                                                    $$->nodetype = new char[type.size()+1];
                                                    strcpy($$->nodetype, type.c_str());
                                                    if (type == "null"){
                                                      error_throw = true;
                                                      fprintf(stderr, "%d |\t Error : Invalid operand(s) with >>. Note : Each operand should of type int or char\n", line);
                                                    }
                                                    else {
                                                      if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                        qid p1 = emit_assignment($$->nodetype, $1->nodetype, $1->place, level, level_id, line);
                                                        qid p2 = emit_assignment($$->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                                        qid t = newtmp($$->nodetype, level, level_id);
                                                        $$->place = t;
                                                        emit(">>", p1, p2, $$->place);
                                                      }
                                                    }
                                                  }
  ;

relational_expression
  : shift_expression                              {$$ = $1;}
  | relational_expression '<' shift_expression    {if ($1->init && $3->init) $$->init = true;
                                                    string type = relat_type($1->nodetype, $3->nodetype);
                                                    if (type != "null"){
                                                      if (type == "*warning"){
                                                        fprintf(stderr, "%d |\t Warning : Comparison between %s and %s\n", line, ($1->nodetype), ($3->nodetype));
                                                        type = "*";
                                                      }
                                                      char* label; string tmp = "< " + type; label = &tmp[0];
                                                      $$ = non_terminal(0, label, $1, $3);
                                                      $$->nodetype = "bool";

                                                      if (type == "*"){
                                                        if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                          qid t = newtmp($$->nodetype, level, level_id);
                                                          $$->place = t;
                                                          emit("<ptr", $1->place, $3->place, $$->place);
                                                        }
                                                      }
                                                      else if (type == "int"){
                                                        if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                          qid p1 = emit_assignment(type, $1->nodetype, $1->place, level, level_id, line);
                                                          qid p2 = emit_assignment(type, $3->nodetype, $3->place, level, level_id, line);
                                                          qid t = newtmp($$->nodetype, level, level_id);
                                                          $$->place = t;
                                                          emit("<int", p1, p2, $$->place);
                                                        }
                                                      }
                                                      else if (type == "float"){
                                                        if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                          qid p1 = emit_assignment(type, $1->nodetype, $1->place, level, level_id, line);
                                                          qid p2 = emit_assignment(type, $3->nodetype, $3->place, level, level_id, line);
                                                          qid t = newtmp($$->nodetype, level, level_id);
                                                          $$->place = t;
                                                          fprintf(stderr, "%d |\t Warning : Comparison between floats\n", line);
                                                          emit("<float", p1, p2, $$->place);
                                                        }
                                                      }
                                                    }
                                                    else {
                                                      error_throw = true;
                                                      fprintf(stderr, "%d |\t Error : Invalid operand(s) with <\n", line);
                                                      $$ = non_terminal(0, $2, $1, $3);
                                                      $$->nodetype = new char[type.size()+1];
                                                      strcpy($$->nodetype, type.c_str());
                                                    }
                                                  }
  | relational_expression '>' shift_expression    {if ($1->init && $3->init) $$->init = true;
                                                    string type = relat_type($1->nodetype, $3->nodetype);
                                                    if (type != "null"){
                                                      if (type == "*warning"){
                                                        fprintf(stderr, "%d |\t Warning : Comparison between %s and %s\n", line, ($1->nodetype), ($3->nodetype));
                                                        type = "*";
                                                      }
                                                      char* label; string tmp = "> " + type; label = &tmp[0];
                                                      $$ = non_terminal(0, label, $1, $3);
                                                      $$->nodetype = "bool";

                                                      if (type == "*"){
                                                        if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                          qid t = newtmp($$->nodetype, level, level_id);
                                                          $$->place = t;
                                                          emit(">ptr", $1->place, $3->place, $$->place);
                                                        }
                                                      }
                                                      else if (type == "int"){
                                                        if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                          qid p1 = emit_assignment(type, $1->nodetype, $1->place, level, level_id, line);
                                                          qid p2 = emit_assignment(type, $3->nodetype, $3->place, level, level_id, line);
                                                          qid t = newtmp($$->nodetype, level, level_id);
                                                          $$->place = t;
                                                          emit(">int", p1, p2, $$->place);
                                                        }
                                                      }
                                                      else if (type == "float"){
                                                        if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                          qid p1 = emit_assignment(type, $1->nodetype, $1->place, level, level_id, line);
                                                          qid p2 = emit_assignment(type, $3->nodetype, $3->place, level, level_id, line);
                                                          qid t = newtmp($$->nodetype, level, level_id);
                                                          $$->place = t;
                                                          fprintf(stderr, "%d |\t Warning : Comparison between floats\n", line);
                                                          emit(">float", p1, p2, $$->place);
                                                        }
                                                      }
                                                    }
                                                    else {
                                                      error_throw = true;
                                                      fprintf(stderr, "%d |\t Error : Invalid operand(s) with >\n", line);
                                                      $$ = non_terminal(0, $2, $1, $3);
                                                      $$->nodetype = new char[type.size()+1];
                                                      strcpy($$->nodetype, type.c_str());
                                                    }
                                                  }
  | relational_expression LE_OP shift_expression  {if ($1->init && $3->init) $$->init = true;
                                                    string type = relat_type($1->nodetype, $3->nodetype);
                                                    if (type != "null"){
                                                      if (type == "*warning"){
                                                        fprintf(stderr, "%d |\t Warning : Comparison between %s and %s\n", line, ($1->nodetype), ($3->nodetype));
                                                        type = "*";
                                                      }
                                                      char* label; string tmp = "<= " + type; label = &tmp[0];
                                                      $$ = non_terminal(0, label, $1, $3);
                                                      $$->nodetype = "bool";

                                                      if (type == "*"){
                                                        if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                          qid t = newtmp($$->nodetype, level, level_id);
                                                          $$->place = t;
                                                          emit("<=ptr", $1->place, $3->place, $$->place);
                                                        }
                                                      }
                                                      else if (type == "int"){
                                                        if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                          qid p1 = emit_assignment(type, $1->nodetype, $1->place, level, level_id, line);
                                                          qid p2 = emit_assignment(type, $3->nodetype, $3->place, level, level_id, line);
                                                          qid t = newtmp($$->nodetype, level, level_id);
                                                          $$->place = t;
                                                          emit("<=int", p1, p2, $$->place);
                                                        }
                                                      }
                                                      else if (type == "float"){
                                                        if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                          qid p1 = emit_assignment(type, $1->nodetype, $1->place, level, level_id, line);
                                                          qid p2 = emit_assignment(type, $3->nodetype, $3->place, level, level_id, line);
                                                          qid t = newtmp($$->nodetype, level, level_id);
                                                          $$->place = t;
                                                          fprintf(stderr, "%d |\t Warning : Comparison between floats\n", line);
                                                          emit("<=float", p1, p2, $$->place);
                                                        }
                                                      }
                                                    }
                                                    else {
                                                      error_throw = true;
                                                      fprintf(stderr, "%d |\t Error : Invalid operand(s) with <=\n", line);
                                                      $$ = non_terminal(0, $2, $1, $3);
                                                      $$->nodetype = new char[type.size()+1];
                                                      strcpy($$->nodetype, type.c_str());
                                                    }
                                                  }
  | relational_expression GE_OP shift_expression  {if ($1->init && $3->init) $$->init = true;
                                                    string type = relat_type($1->nodetype, $3->nodetype);
                                                    if (type != "null"){
                                                      if (type == "*warning"){
                                                        fprintf(stderr, "%d |\t Warning : Comparison between %s and %s\n", line, ($1->nodetype), ($3->nodetype));
                                                        type = "*";
                                                      }
                                                      char* label; string tmp = ">= " + type; label = &tmp[0];
                                                      $$ = non_terminal(0, label, $1, $3);
                                                      $$->nodetype = "bool";

                                                      if (type == "*"){
                                                        if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                          qid t = newtmp($$->nodetype, level, level_id);
                                                          $$->place = t;
                                                          emit(">=ptr", $1->place, $3->place, $$->place);
                                                        }
                                                      }
                                                      else if (type == "int"){
                                                        if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                          qid p1 = emit_assignment(type, $1->nodetype, $1->place, level, level_id, line);
                                                          qid p2 = emit_assignment(type, $3->nodetype, $3->place, level, level_id, line);
                                                          qid t = newtmp($$->nodetype, level, level_id);
                                                          $$->place = t;
                                                          emit(">=int", p1, p2, $$->place);
                                                        }
                                                      }
                                                      else if (type == "float"){
                                                        if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                          qid p1 = emit_assignment(type, $1->nodetype, $1->place, level, level_id, line);
                                                          qid p2 = emit_assignment(type, $3->nodetype, $3->place, level, level_id, line);
                                                          qid t = newtmp($$->nodetype, level, level_id);
                                                          $$->place = t;
                                                          fprintf(stderr, "%d |\t Warning : Comparison between floats\n", line);
                                                          emit(">=float", p1, p2, $$->place);
                                                        }
                                                      }
                                                    }
                                                    else {
                                                      error_throw = true;
                                                      fprintf(stderr, "%d |\t Error : Invalid operand(s) with >=\n", line);
                                                      $$ = non_terminal(0, $2, $1, $3);
                                                      $$->nodetype = new char[type.size()+1];
                                                      strcpy($$->nodetype, type.c_str());
                                                    }
                                                  }
  ;

equality_expression
  : relational_expression                           {$$ = $1;}
  | equality_expression EQ_OP relational_expression {if ($1->init && $3->init) $$->init = true;
                                                      string type = relat_type($1->nodetype, $3->nodetype);
                                                      if (type != "null"){
                                                        if (type == "*warning"){
                                                          fprintf(stderr, "%d |\t Warning : Comparison between %s and %s\n", line, ($1->nodetype), ($3->nodetype));
                                                          type = "*";
                                                        }
                                                        char* label; string tmp = "== " + type; label = &tmp[0];
                                                        $$ = non_terminal(0, label, $1, $3);
                                                        $$->nodetype = "bool";

                                                        if (type == "*"){
                                                          if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                            qid t = newtmp($$->nodetype, level, level_id);
                                                            $$->place = t;
                                                            emit("==ptr", $1->place, $3->place, $$->place);
                                                          }
                                                        }
                                                        else if (type == "int"){
                                                          if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                            qid p1 = emit_assignment(type, $1->nodetype, $1->place, level, level_id, line);
                                                            qid p2 = emit_assignment(type, $3->nodetype, $3->place, level, level_id, line);
                                                            qid t = newtmp($$->nodetype, level, level_id);
                                                            $$->place = t;
                                                            emit("==int", p1, p2, $$->place);
                                                          }
                                                        }
                                                        else if (type == "float"){
                                                          if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                            qid p1 = emit_assignment(type, $1->nodetype, $1->place, level, level_id, line);
                                                            qid p2 = emit_assignment(type, $3->nodetype, $3->place, level, level_id, line);
                                                            qid t = newtmp($$->nodetype, level, level_id);
                                                            $$->place = t;
                                                            fprintf(stderr, "%d |\t Warning : Comparison between floats\n", line);
                                                            emit("==float", p1, p2, $$->place);
                                                          }
                                                        }
                                                      }
                                                      else {
                                                        error_throw = true;
                                                        fprintf(stderr, "%d |\t Error : Invalid operand(s) with ==\n", line);
                                                        $$ = non_terminal(0, $2, $1, $3);
                                                        $$->nodetype = new char[type.size()+1];
                                                        strcpy($$->nodetype, type.c_str());
                                                      }
                                                    }
  | equality_expression NE_OP relational_expression {if ($1->init && $3->init) $$->init = true;
                                                      string type = relat_type($1->nodetype, $3->nodetype);
                                                      if (type != "null"){
                                                        if (type == "*warning"){
                                                          fprintf(stderr, "%d |\t Warning : Comparison between %s and %s\n", line, ($1->nodetype), ($3->nodetype));
                                                          type = "*";
                                                        }
                                                        char* label; string tmp = "!= " + type; label = &tmp[0];
                                                        $$ = non_terminal(0, label, $1, $3);
                                                        $$->nodetype = "bool";

                                                        if (type == "*"){
                                                          if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                            qid t = newtmp($$->nodetype, level, level_id);
                                                            $$->place = t;
                                                            emit("!=ptr", $1->place, $3->place, $$->place);
                                                          }
                                                        }
                                                        else if (type == "int"){
                                                          if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                            qid p1 = emit_assignment(type, $1->nodetype, $1->place, level, level_id, line);
                                                            qid p2 = emit_assignment(type, $3->nodetype, $3->place, level, level_id, line);
                                                            qid t = newtmp($$->nodetype, level, level_id);
                                                            $$->place = t;
                                                            emit("!=int", p1, p2, $$->place);
                                                          }
                                                        }
                                                        else if (type == "float"){
                                                          if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                            qid p1 = emit_assignment(type, $1->nodetype, $1->place, level, level_id, line);
                                                            qid p2 = emit_assignment(type, $3->nodetype, $3->place, level, level_id, line);
                                                            qid t = newtmp($$->nodetype, level, level_id);
                                                            $$->place = t;
                                                            fprintf(stderr, "%d |\t Warning : Comparison between floats\n", line);
                                                            emit("!=float", p1, p2, $$->place);
                                                          }
                                                        }
                                                      }
                                                      else {
                                                        error_throw = true;
                                                        fprintf(stderr, "%d |\t Error : Invalid operand(s) with !=\n", line);
                                                        $$ = non_terminal(0, $2, $1, $3);
                                                        $$->nodetype = new char[type.size()+1];
                                                        strcpy($$->nodetype, type.c_str());
                                                      }
                                                    }
  ;

and_expression
  : equality_expression                     {$$ = $1;}
  | and_expression '&' equality_expression  {if ($1->init && $3->init) $$->init = true;
                                              string type = bit_type($1->nodetype, $3->nodetype);
                                              if (type != "null"){
                                                $$ = non_terminal(0, "& int", $1, $3);
                                                $$->nodetype = new char[type.size()+1];
                                                strcpy($$->nodetype, type.c_str());

                                                if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                  qid p1 = emit_assignment($$->nodetype, $1->nodetype, $1->place, level, level_id, line);
                                                  qid p2 = emit_assignment($$->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                                  qid t = newtmp($$->nodetype, level, level_id);
                                                  $$->place = t;
                                                  emit("&", p1, p2, $$->place);
                                                }
                                              }
                                              else {
                                                error_throw = true;
                                                fprintf(stderr, "%d |\t Error : Invalid operand(s) with &. Note : Each operand should of type int, char or bool\n", line);
                                                $$ = non_terminal(0, $2, $1, $3);
                                                $$->nodetype = new char[type.size()+1];
                                                strcpy($$->nodetype, type.c_str());
                                              }
                                            }
  ;

exclusive_or_expression
  : and_expression                              {$$ = $1;}
  | exclusive_or_expression '^' and_expression  {if ($1->init && $3->init) $$->init = true;
                                                  string type = bit_type($1->nodetype, $3->nodetype);
                                                  if (type != "null"){
                                                    $$ = non_terminal(0, "^ int", $1, $3);
                                                    $$->nodetype = new char[type.size()+1];
                                                    strcpy($$->nodetype, type.c_str());

                                                    if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                      qid p1 = emit_assignment($$->nodetype, $1->nodetype, $1->place, level, level_id, line);
                                                      qid p2 = emit_assignment($$->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                                      qid t = newtmp($$->nodetype, level, level_id);
                                                      $$->place = t;
                                                      emit("^", p1, p2, $$->place);
                                                    }
                                                  }
                                                  else {
                                                    error_throw = true;
                                                    fprintf(stderr, "%d |\t Error : Invalid operand(s) with ^. Note : Each operand should of type int, char or bool\n", line);
                                                    $$ = non_terminal(0, $2, $1, $3);
                                                    $$->nodetype = new char[type.size()+1];
                                                    strcpy($$->nodetype, type.c_str());
                                                  }
                                                }
  ;

inclusive_or_expression
  : exclusive_or_expression                              {$$ = $1;}
  | inclusive_or_expression '|' exclusive_or_expression  {if ($1->init && $3->init) $$->init = true;
                                                          string type = bit_type($1->nodetype, $3->nodetype);
                                                          if (type != "null"){
                                                            $$ = non_terminal(0, "| int", $1, $3);
                                                            $$->nodetype = new char[type.size()+1];
                                                            strcpy($$->nodetype, type.c_str());

                                                            if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                              qid p1 = emit_assignment($$->nodetype, $1->nodetype, $1->place, level, level_id, line);
                                                              qid p2 = emit_assignment($$->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                                              qid t = newtmp($$->nodetype, level, level_id);
                                                              $$->place = t;
                                                              emit("|", p1, p2, $$->place);
                                                            }
                                                          }
                                                          else {
                                                            error_throw = true;
                                                            fprintf(stderr, "%d |\t Error : Invalid operand(s) with |. Note : Each operand should of type int, char or bool\n", line);
                                                            $$ = non_terminal(0, $2, $1, $3);
                                                            $$->nodetype = new char[type.size()+1];
                                                            strcpy($$->nodetype, type.c_str());
                                                          }
                                                          }
  ;

marker1
  : logical_and_expression AND_OP {
                                    $$ = $1;
                                    if(!error_throw && $1->truelist == NULL){
                                      int tmp1 = emit("GOTO IF", NULL, $1->place, NULL);
                                      int tmp2 = emit("GOTO", NULL, NULL, NULL);
                                      ($$->truelist) = insert($$->truelist, tmp1);
                                      ($$->falselist) = insert($$->falselist, tmp2);
                                    }
                                  }
  ;

logical_and_expression
  : inclusive_or_expression             {$$ = $1;}
  | marker1 M inclusive_or_expression   {$$ = non_terminal(0, "&&", $1, $3);
                                          if ($1->init && $3->init) $$->init = true;
                                          $$->nodetype = "bool";

                                          if (!error_throw){
                                            $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                            if($3->truelist == NULL){
                                              int tmp1 = emit("GOTO IF", NULL, $3->place, NULL);
                                              int tmp2 = emit("GOTO", NULL, NULL, NULL);
                                              ($3->truelist) = insert($3->truelist, tmp1);
                                              ($3->falselist) = insert($3->falselist, tmp2);
                                            }
                                            backpatch($1->truelist, $2);
                                            $$->truelist = copy($3->truelist);
                                            $$->falselist = merge($1->falselist, $3->falselist);
                                          }
                                        }
  ;

marker2
  : logical_or_expression OR_OP {
                                    $$ = $1;
                                    if(!error_throw && $1->truelist == NULL){
                                      int tmp1 = emit("GOTO IF", NULL, $1->place, NULL);
                                      int tmp2 = emit("GOTO", NULL, NULL, NULL);
                                      ($$->truelist) = insert($$->truelist, tmp1);
                                      ($$->falselist) = insert($$->falselist, tmp2);
                                    }
                                  }
  ;

logical_or_expression
  : logical_and_expression            {$$ = $1;}
  | marker2 M logical_and_expression  {$$ = non_terminal(0, "||", $1, $3);
                                        if ($1->init && $3->init) $$->init = true;
                                        $$->nodetype = "bool";

                                        if (!error_throw){
                                          $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                          if($3->truelist == NULL){
                                            int tmp1 = emit("GOTO IF", NULL, $3->place, NULL);
                                            int tmp2 = emit("GOTO", NULL, NULL, NULL);
                                            ($3->truelist) = insert($3->truelist, tmp1);
                                            ($3->falselist) = insert($3->falselist, tmp2);
                                          }
                                          backpatch($1->falselist, $2);
                                          $$->falselist = copy($3->falselist);
                                          $$->truelist = merge($1->truelist, $3->truelist);
                                        }
                                      }
  ;

marker3
  : logical_or_expression '?'         {$$ = $1;
                                        if(!error_throw && $1->truelist == NULL){
                                          int tmp1 = emit("GOTO IF", NULL, $1->place, NULL);
                                          int tmp2 = emit("GOTO", NULL, NULL, NULL);
                                          ($$->truelist) = insert($$->truelist, tmp1);
                                          ($$->falselist) = insert($$->falselist, tmp2);
                                        }
                                      }

conditional_expression
  : logical_or_expression                                           {$$ = $1;}
  | marker3 M expression ':' N M conditional_expression               {$$ = non_terminal(1, "conditional_expression", $1, $3, $7);
                                                                      string type = cond_type($3->nodetype, $7->nodetype);
                                                                      if (type != "null"){
                                                                        $$->nodetype = new char[type.size()+1];
                                                                        strcpy($$->nodetype, type.c_str());

                                                                        if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                          backpatch($1->truelist, $2);
                                                                          backpatch($1->falselist, $6);

                                                                          qid p1 = emit_assignment($$->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                                                          qid p2 = emit_assignment($$->nodetype, $7->nodetype, $7->place, level, level_id, line);
                                                                          qid t = newtmp($$->nodetype, level, level_id);
                                                                          $$->place = t;

                                                                          int k0, k1, k2, k3;
                                                                          if ($7->truelist || $7->falselist){
                                                                            if ($7->truelist && $7->falselist){
                                                                              k0 = emit("=", NULL, NULL, $$->place);
                                                                              patch_constant("1", k0);
                                                                              k1 = emit("GOTO", NULL, NULL, NULL);
                                                                              k2 = emit("=", NULL, NULL, $$->place);
                                                                              patch_constant("0", k2);
                                                                              k3 = emit("GOTO", NULL, NULL, NULL);
                                                                              $$->nextlist = insert($$->nextlist, k1);
                                                                              $$->nextlist = insert($$->nextlist, k3);
                                                                            }
                                                                            else if ($7->truelist) {
                                                                              k0 = emit("=", NULL, NULL, $$->place);
                                                                              patch_constant("1", k0);
                                                                              k1 = emit("GOTO", NULL, NULL, NULL);
                                                                              $$->nextlist = insert($$->nextlist, k1);
                                                                            }
                                                                            else if ($7->falselist){
                                                                              k2 = emit("=", NULL, NULL, $$->place);
                                                                              patch_constant("0", k2);
                                                                              k3 = emit("GOTO", NULL, NULL, NULL);
                                                                              $$->nextlist = insert($$->nextlist, k3);
                                                                            }
                                                                          }
                                                                          else {
                                                                            k0 = emit("=", NULL, p2, $$->place);
                                                                            k1 = emit("GOTO", NULL, NULL, NULL);
                                                                            $$->nextlist = insert($$->nextlist, k1);
                                                                          }
                                                                          backpatch($7->truelist, k0);
                                                                          backpatch($7->falselist, k2);
                                                                          backpatch($7->nextlist, k0);

                                                                          backpatch($5->nextlist, nextinstr());

                                                                          int k4, k5, k6;
                                                                          if ($3->truelist || $3->falselist){
                                                                            if ($3->truelist && $3->falselist){
                                                                              k4 = emit("=", NULL, NULL, $$->place);
                                                                              patch_constant("1", k4);
                                                                              k5 = emit("GOTO", NULL, NULL, NULL);
                                                                              k6 = emit("=", NULL, NULL, $$->place);
                                                                              patch_constant("0", k6);
                                                                              $$->nextlist = insert($$->nextlist, k5);
                                                                            }
                                                                            else if ($7->truelist) {
                                                                              k4 = emit("=", NULL, NULL, $$->place);
                                                                              patch_constant("1", k4);
                                                                            }
                                                                            else if ($7->falselist){
                                                                              k6 = emit("=", NULL, NULL, $$->place);
                                                                              patch_constant("0", k6);
                                                                            }
                                                                          }
                                                                          else {
                                                                            k4 = emit("=", NULL, p1, $$->place);
                                                                          }
                                                                          backpatch($3->truelist, k4);
                                                                          backpatch($3->falselist, k6);
                                                                          backpatch($3->nextlist, k4);
                                                                        }
                                                                      }
                                                                      else {
                                                                        error_throw = true;
                                                                        $$->nodetype = new char[type.size()+1];
                                                                        strcpy($$->nodetype, type.c_str());
                                                                        fprintf(stderr, "%d |\t Error : Type mismatch in ternary expression\n", line);
                                                                      }
                                                                      if ($1->init && $3->init && $7->init) $$->init = true;
                                                                    }
  ;

assignment_expression
  : conditional_expression                                      {$$ = $1;}
  | unary_expression assignment_operator assignment_expression  {$$ = non_terminal(0, $2, $1, $3);
                                                                  string label = string($2);
                                                                  string type = assign_type($1->nodetype, $3->nodetype, label);
                                                                  string tmp($1->nodetype);
                                                                  if (type == "0"){
                                                                    $$->nodetype = $1->nodetype;
                                                                    fprintf(stderr, "%d |\t Warning : Incompatible pointer type assignment\n", line);
                                                                  }
                                                                  else if (type == "1"){
                                                                    $$->nodetype = $1->nodetype;
                                                                  }
                                                                  else if (tmp == "void"){
                                                                    $$->nodetype = $1->nodetype;
                                                                    error_throw = true;
                                                                    fprintf(stderr, "%d |\t Error : Variable or field %s of type void is being assigned a value\n", line, ($1->symbol));
                                                                  }
                                                                  else {
                                                                    error_throw = true;
                                                                    $$->nodetype = new char[type.size()+1];
                                                                    strcpy($$->nodetype, type.c_str());
                                                                    string tmp($3->nodetype);
                                                                    if (tmp != "null") fprintf(stderr, "%d |\t Error : Incompatible type conversion from %s to %s\n", line, ($3->nodetype), ($1->nodetype));
                                                                  }

                                                                  if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                    $$->place = $1->place;
                                                                    $$->address = $1->address;

                                                                    if ($3->truelist || $3->falselist){
                                                                      qid tmp = newtmp("bool", level, level_id);
                                                                      if ($3->truelist && $3->falselist){
                                                                        int k0 = emit("=", NULL, NULL, tmp);
                                                                        patch_constant("1", k0);
                                                                        int k1 = emit("GOTO", NULL, NULL, NULL);
                                                                        int k2 = emit("=", NULL, NULL, tmp);
                                                                        patch_constant("0", k2);
                                                                        backpatch($3->truelist, k0);
                                                                        backpatch($3->falselist, k2);
                                                                        $3->nextlist = insert($3->nextlist, k1);
                                                                        backpatch($3->nextlist, nextinstr());
                                                                      }
                                                                      else if ($3->truelist){
                                                                        int k0 = emit("=", NULL, NULL, tmp);
                                                                        patch_constant("1", k0);
                                                                        backpatch($3->truelist, k0);
                                                                      }
                                                                      else if ($3->falselist){
                                                                        int k0 = emit("=", NULL, NULL, tmp);
                                                                        patch_constant("0", k0);
                                                                        backpatch($3->falselist, k0);
                                                                      }

                                                                      if (label == "="){
                                                                        qid tmp1 = emit_assignment($1->nodetype, "bool", tmp, level, level_id, line);
                                                                        if ($$->address){
                                                                          int k = emit("* =", NULL, tmp1, $$->address);
                                                                        }
                                                                        else {
                                                                          int k = emit("=", NULL, tmp1, $$->place);
                                                                        }
                                                                      }
                                                                      else {
                                                                        emit_assignment_multi(label, $1->nodetype, "bool", $1->place, tmp, $1->address, level, level_id, line);
                                                                      }
                                                                    }
                                                                    else {
                                                                      backpatch($3->nextlist, nextinstr());
                                                                      if (label == "="){
                                                                        qid tmp = emit_assignment($1->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                                                        if ($$->address){
                                                                          int k = emit("* =", NULL, tmp, $$->address);
                                                                        }
                                                                        else {
                                                                          int k = emit("=", NULL, tmp, $$->place);
                                                                        }
                                                                      }
                                                                      else {
                                                                        emit_assignment_multi(label, $1->nodetype, $3->nodetype, $1->place, $3->place, $1->address, level, level_id, line);
                                                                      }
                                                                    }
                                                                  }
                                                                }
  ;

assignment_operator
  : '='         {$$ = "=";}
  | MUL_ASSIGN  {$$ = "*=";}
  | DIV_ASSIGN  {$$ = "/=";}
  | MOD_ASSIGN  {$$ = "%=";}
  | ADD_ASSIGN  {$$ = "+=";}
  | SUB_ASSIGN  {$$ = "-=";}
  | LEFT_ASSIGN {$$ = "<<=";}
  | RIGHT_ASSIGN{$$ = ">>=";}
  | AND_ASSIGN  {$$ = "&=";}
  | XOR_ASSIGN  {$$ = "^=";}
  | OR_ASSIGN   {$$ = "|=";}
  ;

expression
  : assignment_expression                 {$$ = $1; }
  | expression ',' M assignment_expression  {$$ = non_terminal(0, "expression", $1, $4);
                                              if (!error_throw){
                                                $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                backpatch($1->nextlist, $3);
                                                $$->nextlist = copy($4->nextlist);
                                              }
                                            }
  ;

constant_expression
  : conditional_expression  {$$ = $1;}
  ;

declaration
  : declaration_specifiers ';'                        {$$ = $1;
                                                        t_name = "";
                                                      }
  | declaration_specifiers init_declarator_list ';'   {$$ = non_terminal(0, "declaration", $1, $2);
                                                        t_name = "";
                                                        if ($2->expr_type == 2){
                                                          if (decl_track.find($2->symbol) != decl_track.end()){
                                                            error_throw = true;
                                                            fprintf(stderr, "%d |\t Error : Multiple declarations/definitions of function %s\n", line, ($2->symbol));
                                                          }
                                                          else {
                                                            decl_track.insert($2->symbol);
                                                          }
                                                        }
                                                        if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                          $$->nextlist = copy($2->nextlist);
                                                        }
                                                      }
  ;

declaration_specifiers
  : storage_class_specifier                           {$$ = $1;
                                                        error_throw = true;
                                                        fprintf(stderr, "%d |\t Error : Storage class specifier not implemented yet\n", line);
                                                      }
  | storage_class_specifier declaration_specifiers    {$$ = non_terminal(0, "declaration_specifiers", $1, $2);
                                                        error_throw = true;
                                                        fprintf(stderr, "%d |\t Error : Storage class specifier not implemented yet\n", line);
                                                      }
  | type_specifier                                    {$$ = $1;}
  | type_specifier declaration_specifiers             {$$ = non_terminal(0, "declaration_specifiers", $1, $2);
                                                        string type = string($1->nodetype) + " " + string($2->nodetype);
                                                        $$->nodetype = new char[type.size()+1];
                                                        strcpy($$->nodetype, type.c_str());
                                                      }
  | type_qualifier                                    {$$ = $1;
                                                        error_throw = true;
                                                        fprintf(stderr, "%d |\t Error : Type qualifier not implemented yet\n", line);
                                                      }
  | type_qualifier declaration_specifiers             {$$ = non_terminal(0, "declaration_specifiers", $1, $2);
                                                        error_throw = true;
                                                        fprintf(stderr, "%d |\t Error : Type qualifier not implemented yet\n", line);
                                                      }
  ;

init_declarator_list
  : init_declarator                           {$$ = $1;}
  | init_declarator_list ',' M init_declarator  {$$ = non_terminal(0, "init_declarator_list", $1, $4);
                                                  if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                    backpatch($1->nextlist, $3);
                                                    $$->nextlist = copy($4->nextlist);
                                                  }
                                                }
  ;

init_declarator
  : declarator                  {$$ = $1;
                                  if ($1->expr_type == 1 || $1->expr_type == 15){
                                    string type = $1->nodetype;
                                    if (lookup_decl($1->symbol, level, level_id[level])){
                                      error_throw = true;
                                      fprintf(stderr, "%d |\t Error : Redeclaration of symbol %s\n", line, ($1->symbol));
                                    }
                                    else if (type == "void"){
                                      error_throw = true;
                                      fprintf(stderr, "%d |\t Error : Variable or field %s declared as type void\n", line, ($1->symbol));
                                    }
                                    else {
                                      insert_entry($1->symbol, $1->nodetype, $1->size, 0, false, level, level_id[level]);
                                      $$->init = false;
                                      if ($1->expr_type == 15){
                                        set_arr_flag($$->symbol, level, level_id[level]);
                                      }
                                    }
                                    if (!error_throw){
                                      qid t = lookup_use($$->symbol, level, level_id);
                                      $$->place = t;
                                    }
                                  }
                                }
  | declarator '=' initializer  {$$ = non_terminal(0, $2, $1, $3);
                                  if ($1->expr_type == 1 || $1->expr_type == 15){
                                    $$->symbol = $1->symbol;
                                    string chk = is_valid($1->nodetype, $3->nodetype);
                                    string type = $1->nodetype;
                                    string type2 = $3->nodetype;
                                    if (lookup_decl($1->symbol, level, level_id[level])){
                                      error_throw = true;
                                      fprintf(stderr, "%d |\t Error : Redeclaration of symbol %s\n", line, ($1->symbol));
                                    }
                                    else if (type == "void"){
                                      error_throw = true;
                                      fprintf(stderr, "%d |\t Error : Variable or field %s declared as type void\n", line, ($1->symbol));
                                    }
                                    else if (type2 == "void"){
                                      error_throw = true;
                                      fprintf(stderr, "%d |\t Error : Variable or field %s assigned expression of type void\n", line, ($1->symbol));
                                    }
                                    else if (chk == "null"){
                                      error_throw = true;
                                      fprintf(stderr, "%d |\t Error : Incompatible types when initializing type %s to %s\n", line, ($1->nodetype), ($3->nodetype));
                                    }
                                    else {
                                      if (chk == "0") fprintf(stderr, "%d |\t Warning : Assignment with incompatible pointer types\n", line);
                                      insert_entry($1->symbol, $1->nodetype, $1->size, 0, $3->init, level, level_id[level]);
                                      $$->nodetype = new char[type.size()+1];
                                      strcpy($$->nodetype, type.c_str());
                                      $$->init = true;
                                      if ($1->expr_type == 15){
                                        set_arr_flag($1->symbol, level, level_id[level]);
                                      }
                                    }
                                    if (!error_throw){ 
                                      $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                      qid t = lookup_use($$->symbol, level, level_id);
                                      $$->place = t;
                                      if ($1->expr_type == 15){
                                        fprintf(stderr, "%d |\t Warning : Array initialization not allowed this way, proceeding without initialization\n", line);
                                        lookup_use($1->symbol, level, level_id)->init = false;
                                      }
                                      else {
                                        if ($3->truelist || $3->falselist){
                                          qid tmp = newtmp("bool", level, level_id);
                                          if ($3->truelist && $3->falselist){
                                            int k0 = emit("=", NULL, NULL, tmp);
                                            patch_constant("1", k0);
                                            int k1 = emit("GOTO", NULL, NULL, NULL);
                                            int k2 = emit("=", NULL, NULL, tmp);
                                            patch_constant("0", k2);
                                            backpatch($3->truelist, k0);
                                            backpatch($3->falselist, k2);
                                            $3->nextlist = insert($3->nextlist, k1);
                                            backpatch($3->nextlist, nextinstr());
                                          }
                                          else if ($3->truelist){
                                            int k0 = emit("=", NULL, NULL, tmp);
                                            patch_constant("1", k0);
                                            backpatch($3->truelist, k0);
                                          }
                                          else if ($3->falselist){
                                            int k0 = emit("=", NULL, NULL, tmp);
                                            patch_constant("0", k0);
                                            backpatch($3->falselist, k0);
                                          }

                                          qid tmp1 = emit_assignment($1->nodetype, "bool", tmp, level, level_id, line);
                                          int k = emit("=", NULL, tmp1, $$->place);
                                        }
                                        else {
                                          backpatch($3->nextlist, nextinstr());
                                          qid tmp = emit_assignment($1->nodetype, $3->nodetype, $3->place, level, level_id, line);
                                          int k = emit("=", NULL, tmp, $$->place);
                                        }
                                      }
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
  : VOID     {$$ = terminal($1); t_name = $1; $$->nodetype = "void";}
  | BOOL     {$$ = terminal($1); t_name = $1; $$->nodetype = "bool";}
  | CHAR     {$$ = terminal($1); t_name = $1; $$->nodetype = "char";}
  | SHORT    {$$ = terminal($1);
                error_throw = true;
                fprintf(stderr, "%d |\t Error : \'short'\ datatypes not allowed\n", line);
                $$->nodetype = "null";
                t_name = "null";
              }
  | INT      {$$ = terminal($1); t_name = $1; $$->nodetype = "int";}
  | LONG     {$$ = terminal($1);
                error_throw = true;
                fprintf(stderr, "%d |\t Error : \'long'\ datatypes not allowed\n", line);
                $$->nodetype = "null";
                t_name = "null";
              }
  | FLOAT    {$$ = terminal($1); t_name = $1; $$->nodetype = "float";}
  | DOUBLE   {$$ = terminal($1);
                error_throw = true;
                fprintf(stderr, "%d |\t Error : \'double'\ datatypes not allowed\n", line);
                $$->nodetype = "null";
                t_name = "null";
              }
  | SIGNED   {$$ = terminal($1);
                error_throw = true;
                fprintf(stderr, "%d |\t Error : \'signed'\ datatypes not allowed, try using native int or float, they are signed\n", line);
                $$->nodetype = "null";
                t_name = "null";
              }
  | UNSIGNED {$$ = terminal($1);
                error_throw = true;
                fprintf(stderr, "%d |\t Error : \'unsigned'\ datatypes not allowed\n", line);
                $$->nodetype = "null";
                t_name = "null";
              }
  | struct_or_union_specifier  {$$ = $1; t_name = $1->nodetype; }
  | enum_specifier  {$$ = $1;
                    error_throw = true;
                    fprintf(stderr, "%d |\t Error : Enum not implemented yet\n", line);
                    }
  ;

struct_or_union_specifier
  : struct_or_union IDENTIFIER struct_op_brace struct_declaration_list '}'  {$$ = non_terminal(1, "struct_or_union_specifier", $1, $4, terminal($2));
                                                                              if (lookup_type_decl(string("struct ") + string($2), level, level_id[level])){
                                                                                $$->nodetype = "null";
                                                                                error_throw = true;
                                                                                fprintf(stderr, "%d |\t Error : Struct %s is already defined\n", line, $2);
                                                                              }
                                                                              else {
                                                                                string temp = string("struct ") + string($2);
                                                                                $$->nodetype = new char[temp.size()+1];
                                                                                strcpy($$->nodetype, temp.c_str());
                                                                                insert_type_entry($$->nodetype, $4->size, level, level_id[level]);
                                                                              }
                                                                              t_name = "";
                                                                            }
  | struct_or_union struct_op_brace struct_declaration_list '}'             {$$ = non_terminal(0, "struct_or_union_specifier", $1, $3);
                                                                              struct_id++;
                                                                              string temp = string("struct ") + to_string(struct_id);
                                                                              $$->nodetype = new char[temp.size()+1];
                                                                              strcpy($$->nodetype, temp.c_str());
                                                                              insert_type_entry($$->nodetype, $3->size, level, level_id[level]);
                                                                              t_name = "";
                                                                            }
  | struct_or_union IDENTIFIER                                  {$$ = non_terminal(0, "struct_or_union_specifier", $1, terminal($2));
                                                                  if (lookup_type_use(string("struct ") + string($2), level, level_id)){
                                                                    string temp = string("struct ") + string($2);
                                                                    $$->nodetype = new char[temp.size()+1];
                                                                    strcpy($$->nodetype, temp.c_str());
                                                                  }
                                                                  else {
                                                                    $$->nodetype = "null";
                                                                    error_throw = true;
                                                                    fprintf(stderr, "%d |\t Error : Struct %s not defined\n", line, $2);
                                                                  }
                                                                  t_name = "";
                                                                }
  ;

struct_op_brace
  : '{'   {
            t_name = "";
            set_struct_scope();
          }

struct_or_union
  : STRUCT    {$$ = terminal($1);}
  | UNION     {$$ = terminal($1);
                error_throw = true;
                fprintf(stderr, "%d |\t Error : Union declaration not allowed, do you mean struct ?\n", line);
              }
  ;

struct_declaration_list
  : struct_declaration                          {$$ = $1;}
  | struct_declaration_list struct_declaration  {$$ = non_terminal(0, "struct_declaration_list", $1, $2);
                                                  $$->size = $1->size + $2->size;
                                                }
  ;

struct_declaration
  : specifier_qualifier_list struct_declarator_list ';' {$$ = non_terminal(0, "struct_declaration", $1, $2);
                                                          t_name = "";
                                                          $$->size = $2->size;
                                                        }
  ;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list   {$$ = non_terminal(0, "specifier_qualifier_list", $1, $2);
                                                string type = string($1->nodetype) + " " + string($2->nodetype);
                                                $$->nodetype = new char[type.size()+1];
                                                strcpy($$->nodetype, type.c_str());
                                              }
	| type_specifier                            {$$ = $1; }
	| type_qualifier specifier_qualifier_list   {$$ = non_terminal(0, "specifier_qualifier_list", $1, $2);
                                                error_throw = true;
                                                fprintf(stderr, "%d |\t Error : Type qualifier not implemented yet\n", line);
                                              }
	| type_qualifier                            {$$ = $1;
                                                error_throw = true;
                                                fprintf(stderr, "%d |\t Error : Type qualifier not implemented yet\n", line);
                                              }
	;

struct_declarator_list
	: struct_declarator                             {$$ = $1;}
	| struct_declarator_list ',' struct_declarator  {$$ = non_terminal(0, "struct_declarator_list", $1, $3);
                                                    $$->size = $1->size + $3->size;
                                                  }
	;

struct_declarator
  : declarator                          {$$ = $1;
                                          if (!insert_struct_symbol($$->symbol, $$->nodetype, $$->size)){
                                            error_throw = true;
                                            fprintf(stderr, "%d |\t Error : Redeclaration of a symbol in struct or using char/bool types, which are not allowed due to alignment issues\n", line);
                                          }
                                          if ($1->expr_type == 15){
                                            set_struct_arr_flag($$->symbol);
                                          }
                                        }
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
                                $$->symbol = $2->symbol;
                                if($2->expr_type == 1 || $2->expr_type == 15){
                                  string temp = string($2->nodetype) + string($1->nodetype);
                                  $$->nodetype = new char[temp.size()+1];
                                  strcpy($$->nodetype, temp.c_str());
                                  if ($2->expr_type == 15){
                                    error_throw = true;
                                    fprintf(stderr, "%d |\t Error : Array of pointers not allowed\n", line);
                                  }
                                  else {
                                    $$->size = get_size($$->nodetype, level, level_id);
                                  }
                                  $$->expr_type = $2->expr_type;
                                }
                                if($2->expr_type == 2){
                                  string temp = string($2->nodetype) + string($1->nodetype);
                                  $$->nodetype = new char[temp.size()+1];
                                  strcpy($$->nodetype, temp.c_str());
                                  func_name = string($2->symbol);
                                  if (decl_track.find(func_name) != decl_track.end() && string($$->nodetype) != get_func_ret_type(func_name)){
                                    error_throw = true;
                                    fprintf(stderr, "%d |\t Error : Conflicting return types for function %s\n", line, ($2->symbol));
                                  }
                                  else update_func_type(func_name, level, level_id[level], $$->nodetype);

                                  $$->expr_type = 2;
                                  $$->size = get_size($$->nodetype, level, level_id);
                                }
                                if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                  $$->place = lookup_use($$->symbol, level, level_id);
                                }
                              }
	| direct_declarator         {
                                $$ = $1;
                                if($1->expr_type == 2){
                                  func_name = string($1->symbol);
                                  if (decl_track.find(func_name) != decl_track.end() && string($$->nodetype) != get_func_ret_type(func_name)){
                                    error_throw = true;
                                    fprintf(stderr, "%d |\t Error : Conflicting return types for function %s\n", line, ($1->symbol));
                                  }
                                  if (is_type_struct($1->nodetype)){
                                    error_throw = true;
                                    fprintf(stderr, "%d |\t Error : Return type of a function cannot be struct\n", line);
                                  }
                                }
                                if (!error_throw){
                                  $$->place = lookup_use($$->symbol, level, level_id);
                                }
                              }
	;

direct_declarator
	: IDENTIFIER            {
														$$ = terminal($1);
                            $$->expr_type = 1;
                            string id($1);
                            $$->symbol = new char[id.size()+1];
														strcpy($$->symbol, id.c_str());
                            if (is_valid_type(t_name, level, level_id)){
                              $$->nodetype = new char[t_name.size()+1];
                              strcpy($$->nodetype, t_name.c_str());
                            }
                            else {
                              $$->nodetype = "null";
                              error_throw = true;
                              fprintf(stderr, "%d |\t Error : Invalid type for symbol \'%s\'\n", line, $1);
                            }
														$$->size = get_size(t_name, level, level_id);

                            if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                              qid t = NULL;
                              $$->place = t;
                            }
													}
  | direct_declarator '[' INT_C ']'                 {$$ = non_terminal(0, "direct_declarator", $1, terminal("INT_C"), NULL, NULL, NULL, "[]");
                                                      $$->symbol = $1->symbol;
                                                      if($1->expr_type == 1){
                                                        $$->expr_type = 15;
                                                        string temp = string($1->nodetype) + "*";
                                                        $$->nodetype = new char[temp.size()+1];
                                                        strcpy($$->nodetype, temp.c_str());
                                                      }
                                                      if ($1->expr_type == 1){
                                                        string type($1->nodetype);
                                                        if (type == "void"){
                                                          error_throw = true;
                                                          fprintf(stderr, "%d |\t Error : Variable or field %s declared as type void\n", line, ($1->symbol));
                                                        }
                                                        $$->size = get_size($1->nodetype, level, level_id) * $3->int_val;
                                                      }
                                                      else if ($1->expr_type == 15){
                                                        error_throw = true;
                                                        fprintf(stderr, "%d |\t Error : Multi-dimensional arrays not allowed\n", line);
                                                      }
                                                      if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                        qid t = NULL;
                                                        $$->place = t;
                                                      }
                                                    }
  | direct_declarator '[' constant_expression ']'    {$$ = non_terminal(0, "direct_declarator", $1, terminal("INT_C"), NULL, NULL, NULL, "[]");
                                                      error_throw = true;
                                                      fprintf(stderr, "%d |\t Error : Please provide a constant non-negative int as size to declare array\n", line);
                                                    }
	| direct_declarator '(' empty1 parameter_type_list ')'  {
                                                            $$ = non_terminal(0, "direct_declarator", $1, $4, NULL, NULL, NULL, "()");
                                                            $$->symbol = $1->symbol;
                                                            if($1->expr_type == 1){
                                                              $$->expr_type = 2;
                                                              $$->nodetype = $1->nodetype;
                                                              sym_tab_entry* entry = lookup_use($1->symbol, level, level_id);
                                                              if (make_symbol_table($$->symbol)){
                                                                string type = entry->type;
                                                                if (type.substr(0, 5) == "func "){
                                                                  if (entry->init){
                                                                    error_throw = true;
                                                                    fprintf(stderr, "%d |\t Error : Multiple declarations/definitions of function %s\n", line, ($$->symbol));
                                                                  }
                                                                  else {
                                                                    if (!is_consistent($1->symbol, func_args)){
                                                                      error_throw = true;
                                                                      fprintf(stderr, "%d |\t Error : Conflicting argument types for function %s\n", line, ($$->symbol));
                                                                    }
                                                                  }
                                                                }
                                                                else {
                                                                  error_throw = true;
                                                                  fprintf(stderr, "%d |\t Error : Multiple declarations of symbol %s\n", line, ($$->symbol));
                                                                }
                                                              }
                                                              else {
                                                                insert_entry($1->symbol, "func " + string($$->nodetype), 0, 0, false, level, level_id[level]);
                                                                insert_func_args($1->symbol, func_args);
                                                                if (args_to_scope($$->symbol, func_args, func_symbols)){
                                                                  if (!check_args_constraints($1->symbol, func_args)){
                                                                    error_throw = true;
                                                                    fprintf(stderr, "%d |\t Error : Maximum arguments number is 8 (4 non-float and 4 float)\n", line);
                                                                  }
                                                                }
                                                                else {
                                                                  error_throw = true;
                                                                  fprintf(stderr, "%d |\t Error : Redeclaration of symbols in args of function %s\n", line, ($$->symbol));
                                                                }
                                                              }
                                                              func_args = "";
                                                              func_symbols = "";
                                                              $$->size = get_size($$->nodetype, level, level_id);
                                                              if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                qid t = NULL;
                                                                $$->place = t;
                                                              }
                                                            }
                                                          }
	| direct_declarator '(' empty1 ')'                      {
                                                            $$ = non_terminal(0, "direct_declarator", $1, NULL, NULL, NULL, NULL, "()");
                                                            $$->symbol = $1->symbol;
                                                            if($1->expr_type == 1){
                                                              $$->expr_type = 2;
                                                              $$->nodetype = $1->nodetype;
                                                              sym_tab_entry* entry = lookup_use($1->symbol, level, level_id);
                                                              if (make_symbol_table($$->symbol)){
                                                                string type = entry->type;
                                                                if (type.substr(0, 5) == "func "){
                                                                  if (entry->init){
                                                                    error_throw = true;
                                                                    fprintf(stderr, "%d |\t Error : Multiple declarations/definitions of symbol %s\n", line, ($$->symbol));
                                                                  }
                                                                  else {
                                                                    if (!is_consistent($1->symbol, func_args)){
                                                                      error_throw = true;
                                                                      fprintf(stderr, "%d |\t Error : Conflicting argument types for function %s\n", line, ($$->symbol));
                                                                    }
                                                                  }
                                                                }
                                                                else {
                                                                  error_throw = true;
                                                                  fprintf(stderr, "%d |\t Error : Multiple declarations of symbol %s\n", line, ($$->symbol));
                                                                }
                                                              }
                                                              else {
                                                                insert_entry($1->symbol, "func " + string($$->nodetype), 0, 0, false, level, level_id[level]);
                                                                insert_func_args($1->symbol, "null");
                                                              }
                                                              func_args = "";
                                                              func_symbols = "";
                                                            }
                                                            $$->size = get_size($$->nodetype, level, level_id);
                                                            if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                              qid t = NULL;
                                                              $$->place = t;
                                                            }
                                                          }
	;

empty1
  : %empty{
          t_name = "";
          func_args = "";
          func_symbols = "";
        }
  ;


pointer
  : '*'                             {$$ = terminal($1); string temp = "*";
                                      $$->nodetype = new char[temp.size()+1];
                                      strcpy($$->nodetype, temp.c_str());
                                    }
  | '*' type_qualifier_list         {$$ = non_terminal(0, $1, $2);
                                      error_throw = true;
                                      fprintf(stderr, "%d |\t Error : Type qualifier not implemented yet\n", line);
                                    }
  | '*' pointer                     {$$ = non_terminal(0, $1, $2); string temp = "*" + string($2->nodetype);
                                      $$->nodetype = new char[temp.size()+1];
                                      strcpy($$->nodetype, temp.c_str());
                                      error_throw = true;
                                      fprintf(stderr, "%d |\t Error : Multi-level pointers not allowed\n", line);
                                    }
  | '*' type_qualifier_list pointer {$$ = non_terminal(0, $1, $2, $3);
                                      error_throw = true;
                                      fprintf(stderr, "%d |\t Error : Type qualifier not implemented yet\n", line);
                                    }
  ;

type_qualifier_list
	: type_qualifier                      {$$ = $1;}
	| type_qualifier_list type_qualifier  {$$ = non_terminal(0, "type_qualifier_list", $1, $2);}
	;

parameter_type_list
	: parameter_list                {$$ = $1;}
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
                                              func_symbols = (func_symbols == "") ? string($2->symbol) : func_symbols + "," + string($2->symbol);
                                              func_args = (func_args == "") ?  string($2->nodetype) : func_args + "," + string($2->nodetype);
                                              if (!is_type_int($2->nodetype) && !is_type_float($2->nodetype) && !is_type_bool($2->nodetype) && !is_type_char($2->nodetype) && !is_type_ptr($2->nodetype)){
                                                error_throw = true;
                                                fprintf(stderr, "%d |\t Error : Passing invalid type or a struct as argument to a function\n", line);
                                              }
                                            }
                                          }
	;

type_name
	: specifier_qualifier_list                      {$$ = $1; }
	;

initializer
	: assignment_expression         {$$ = $1; }
	| '{' initializer_list '}'      {$$ = $2;
                                    string temp = string($2->nodetype) + "*"; 
                                    $$->nodetype = new char[temp.size()+1];
                                    strcpy($$->nodetype, temp.c_str());
                                    error_throw = true;
                                    fprintf(stderr, "%d |\t Error : Initializer lists not allowed for array/struct initialisation\n", line);
                                  }
	| '{' initializer_list ',' '}'  {$$ = non_terminal(0, "initializer", $2, NULL, NULL, NULL, NULL, $3);
                                    string temp = string($2->nodetype) + "*"; 
                                    $$->nodetype = new char[temp.size()+1];
                                    strcpy($$->nodetype, temp.c_str());
                                    error_throw = true;
                                    fprintf(stderr, "%d |\t Error : Initializer lists not allowed for array/struct initialisation\n", line);
                                  }
	;

initializer_list
	: initializer                       {$$ = $1; }
	| initializer_list ',' initializer  {
                                        $$ = non_terminal(0, "initializer_list", $1, $3);
                                        $$->nodetype = $1->nodetype;
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

switch_case_marker
  : CASE constant_expression ':'                {
                                                  $$ = $2;
                                                  if ($2->truelist || $2->falselist || $2->nextlist){
                                                    error_throw = true;
                                                    fprintf(stderr, "%d |\t Error : Boolean/Conditional expressions not allowed as case expressions\n", line);
                                                  }
                                                  if (!error_throw){
                                                    qid tmp = switch_expr_stack.top();
                                                    string type = relat_type(tmp->type, $2->nodetype);
                                                    if (type != "null"){
                                                      qid t = newtmp("bool", level, level_id);
                                                      if (type == "*warning"){
                                                        fprintf(stderr, "%d |\t Warning : Comparison between different pointer types in switch case\n", line);
                                                        type = "*";
                                                      }
                                                      if (type == "*"){
                                                        emit("==ptr", $2->place, tmp, t);
                                                      }
                                                      else if (type == "int"){
                                                        qid p = emit_assignment(type, $2->nodetype, $2->place, level, level_id, line);
                                                        qid q = emit_assignment(type, tmp->type, tmp, level, level_id, line);
                                                        emit("==int", p, q, t);
                                                      }
                                                      else if (type == "float"){
                                                        qid p = emit_assignment(type, $2->nodetype, $2->place, level, level_id, line);
                                                        qid q = emit_assignment(type, tmp->type, tmp, level, level_id, line);
                                                        emit("==float", p, q, t);
                                                      }
                                                      int tmp1 = emit("GOTO IF", NULL, t, NULL);
                                                      int tmp2 = emit("GOTO", NULL, NULL, NULL);
                                                      ($$->truelist) = insert($$->truelist, tmp1);
                                                      ($$->falselist) = insert($$->falselist, tmp2);
                                                    }
                                                    else {
                                                      error_throw = true;
                                                      fprintf(stderr, "%d |\t Error : Comparison between incompatible types in switch case\n", line);
                                                    }
                                                  }
                                                }

labeled_statement
	: IDENTIFIER ':' M statement                  {$$ = non_terminal(0, "labeled_statement", terminal($1), $4);
                                                  error_throw = true;
                                                  fprintf(stderr, "%d |\t Error : User goto statements not allowed\n", line);
                                                }
	| switch_case_marker M statement            {$$ = non_terminal(0, "labeled_statement", $1, $3);
                                                if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                  backpatch($1->truelist, $2);
                                                  $$->breaklist = copy($3->breaklist);
                                                  $$->nextlist = merge($3->nextlist, $1->falselist);
                                                  $$->continuelist = copy($3->continuelist);
                                                }
                                              }
	| DEFAULT ':' statement                     {$$ = non_terminal(0, "labeled_statement", terminal($1), $3);
                                                if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                  $$->breaklist= copy($3->breaklist);
                                                  $$->nextlist = copy($3->nextlist);
                                                  $$->continuelist = copy($3->continuelist);
                                                }
                                              }
	;

compound_statement
	: op_brace cl_brace                                   {$$ = terminal("{}");}
	| op_brace statement_list cl_brace                    {$$ = $2; }
	| op_brace declaration_list cl_brace                  {$$ = $2; }
	| op_brace declaration_list M statement_list cl_brace   {$$ = non_terminal(0, "compound_statement", $2, $4);
                                                            if (!error_throw){
                                                              $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                              backpatch($2->nextlist, $3);
                                                              $$->nextlist = copy($4->nextlist);
                                                              $$->truelist = copy($4->truelist);
                                                              $$->falselist = copy($4->falselist);
                                                              $$->continuelist = copy($4->continuelist);
                                                              $$->breaklist = copy($4->breaklist);
                                                            }
                                                          }
	;

op_brace
  : '{'       {
                level++;
                if (level > 1) level_id[level]++;
              }
  ;

cl_brace
  : '}'       {
                level--;
              }

  ;

declaration_list
	: declaration                   {$$ = $1; }
	| declaration_list M declaration  {$$ = non_terminal(0, "declaration_list", $1, $3);
                                      if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                        $$->nextlist = copy($3->nextlist);
                                        backpatch($1->nextlist, $2);
                                      }
                                    }
	;

statement_list
	: statement                 {$$ = $1; }
	| statement_list M statement  {$$ = non_terminal(0, "statement_list", $1, $3);
                                  if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                    backpatch($1->nextlist, $2);
                                    $$->nextlist = copy($3->nextlist);
                                    $$->continuelist = merge($1->continuelist, $3->continuelist);
                                    $$->breaklist = merge($1->breaklist, $3->breaklist);
                                  }
                                }
	;

expression_statement
	: ';'               {$$ = terminal($1); }
	| expression ';'    {$$ = $1; }
	;

if_expression
  : IF '(' expression ')'    {
                              $$ = $3;
                              if(!error_throw && $3->truelist == NULL){
                                int tmp1 = emit("GOTO IF", NULL, $3->place, NULL);
                                int tmp2 = emit("GOTO", NULL, NULL, NULL);
                                ($$->truelist) = insert($$->truelist, tmp1);
                                ($$->falselist) = insert($$->falselist, tmp2);
                                backpatch($3->nextlist, tmp1);
                              }
                            }

  ;

switch_expr_marker
  : expression              {
                              $$ = $1;
                              if ($$->truelist || $$->falselist || $$->nextlist){
                                error_throw = true;
                                fprintf(stderr, "%d |\t Error : Boolean/Conditional expressions not allowed as switch expressions\n", line);
                              }
                              if (!error_throw) switch_expr_stack.push($1->place);
                            }

selection_statement
	: if_expression M statement ELSE N M statement
          {
            $$ = non_terminal(1, "IF (expr) stmt ELSE stmt", $1, $3, $7);
            if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
              backpatch($1->truelist, $2);
              backpatch($1->falselist, $6);
              $$->nextlist = merge($3->nextlist, $7->nextlist);
              $$->nextlist = merge($$->nextlist, $5->nextlist);
              $$->breaklist = merge($3->breaklist, $7->breaklist);
              $$->continuelist = merge($3->continuelist, $7->continuelist);
              if (($$->continuelist || $$->breaklist) && !loop){
                error_throw = true;
                fprintf(stderr, "%d |\t Error : Cannot use continue or break statements inside an if else statement if it is not nested within a loop\n", line);
              }
            }
          }
	| if_expression M statement
          {
            $$ = non_terminal(0, "IF (expr) stmt", $1, $3);
            if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
              backpatch($1->truelist, $2);
              $$->nextlist = merge($1->falselist, $3->nextlist);
              $$->continuelist = copy($3->continuelist);
              $$->breaklist = copy($3->breaklist);
              if (($$->continuelist || $$->breaklist) && !loop){
                error_throw = true;
                fprintf(stderr, "%d |\t Error : Cannot use continue or break statements inside an if statement if it is not nested within a loop\n", line);
              }
            }
          }
	| SWITCH '(' switch_expr_marker ')' statement
          {
            $$ = non_terminal(0, "SWITCH (expr) stmt", $3, $5);
            if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
              $$->nextlist = merge($5->nextlist, $5->breaklist);
              $$->continuelist = copy($5->continuelist);
              switch_expr_stack.pop();
              if ($$->continuelist && !loop){
                error_throw = true;
                fprintf(stderr, "%d |\t Error : Cannot use continue statements inside a switch statement if it is not nested within a loop\n", line);
              }
            }
          }
	;

expr_marker
  : expression    {
                              $$ = $1;
                              if(!error_throw && $1->truelist == NULL){
                                int tmp1 = emit("GOTO IF", NULL, $1->place, NULL);
                                int tmp2 = emit("GOTO", NULL, NULL, NULL);
                                ($$->truelist) = insert($$->truelist, tmp1);
                                ($$->falselist) = insert($$->falselist, tmp2);
                                backpatch($1->nextlist, tmp1);
                              }
                            }

  ;

exprstmt_marker
  : expression_statement    {
                              $$ = $1;
                              if(!error_throw && $1->truelist == NULL){
                                int tmp1 = emit("GOTO IF", NULL, $1->place, NULL);
                                int tmp2 = emit("GOTO", NULL, NULL, NULL);
                                ($$->truelist) = insert($$->truelist, tmp1);
                                ($$->falselist) = insert($$->falselist, tmp2);
                                backpatch($1->nextlist, tmp1);
                              }
                            }

  ;

iteration_statement
	: WHILE '(' M expr_marker ')' M loop_mark statement
        {
          $$ = non_terminal(0, "WHILE (expr) stmt", $4, $8);
          loop--;
          if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
            int k = emit("GOTO", NULL, NULL, NULL);
            ($8->nextlist) = insert($8->nextlist, k);
            backpatch($8->nextlist, $3);
            backpatch($8->continuelist, $3);
            backpatch($4->truelist, $6);
            $$->nextlist = merge($4->falselist, $8->breaklist);
          }
        }
	| DO M loop_mark statement WHILE '(' M expr_marker ')' ';'
        {
          $$ = non_terminal(0, "DO stmt WHILE (expr)", $4, $8);
          loop--;
          if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
            backpatch($4->continuelist, $7);
            backpatch($4->nextlist, $7);
            backpatch($8->truelist, $2);
            $$->nextlist = merge($4->breaklist, $8->falselist);
          }
        }
	| FOR '(' expression_statement M exprstmt_marker ')' M loop_mark statement
        {
          $$ = non_terminal(1, "FOR (expr_stmt expr_stmt) stmt", $3, $5, $9);
          loop--;
          if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
            int k = emit("GOTO", NULL, NULL, NULL);
            ($9->nextlist) = insert($9->nextlist, k);
            backpatch($5->truelist, $7);
            backpatch($3->nextlist, $4);
            backpatch($9->nextlist, $4);
            backpatch($9->continuelist, $4);
            $$->nextlist = merge($5->falselist, $9->breaklist);
          }
        }
	| FOR '(' expression_statement M exprstmt_marker M expression N ')' M loop_mark statement
        {
          $$ = non_terminal(1, "FOR (expr_stmt expr_stmt expr) stmt", $3, $5, $7, $12);
          if ($7->truelist || $7->falselist){
            error_throw = true;
            fprintf(stderr, "%d |\t Error : Do not use boolean update expressions in for loops\n", line);
          }
          loop--;
          if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
            int k = emit("GOTO", NULL, NULL, NULL);
            ($12->nextlist) = insert($12->nextlist, k);
            backpatch($12->nextlist, $6);
            backpatch($12->continuelist, $6);
            backpatch($5->truelist, $10);
            backpatch($3->nextlist, $4);
            $$->nextlist = merge($5->falselist, $12->breaklist);
            backpatch($8->nextlist, $4);
            backpatch($7->nextlist, $4);
          }
        }
	;

loop_mark
  : %empty          { loop++; }

jump_statement
	: GOTO IDENTIFIER ';'       {
                                $$ = non_terminal(0, "jump_stmt", terminal($1), terminal($2));
                                error_throw = true;
                                fprintf(stderr, "%d |\t Error : User goto statements not allowed\n", line);
                              }
	| CONTINUE ';'
        {
          $$ = terminal($1);
          if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
            int k = emit("GOTO", NULL, NULL, NULL);
            ($$->continuelist) = insert($$->continuelist, k);
          }
        }
	| BREAK ';'
        {
          $$ = terminal($1);
          if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
            int k = emit("GOTO", NULL, NULL, NULL);
            ($$->breaklist) = insert($$->breaklist, k);
          }
        }
	| RETURN ';'                {$$ = terminal($1);
                                string type = get_func_ret_type(func_name);
                                if (type != "void" && type != "null"){
                                  error_throw = true;
                                  fprintf(stderr, "%d |\t Error : Function %s of type non-void is not returning a value\n", line, func_name.c_str());
                                }else{
                                  if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                    emit("RETURN", NULL, NULL, NULL);
                                  }
                                }
                              }
	| RETURN expression ';'     {$$ = non_terminal(0, "jump_stmt", terminal($1), $2);
                                string type = get_func_ret_type(func_name);
                                if (type == "void"){
                                  error_throw = true;
                                  fprintf(stderr, "%d |\t Error : Function %s of type void is returning a value\n", line, func_name.c_str());
                                }
                                else if (type != "null") {
                                  string chk = is_valid(type, $2->nodetype);
                                  if (chk == "null"){
                                    error_throw = true;
                                    fprintf(stderr, "%d |\t Error : Returned expression of type %s cannot be typecasted to %s\n", line, ($2->nodetype), type.c_str());
                                  }
                                  else {
                                    if (chk == "0") fprintf(stderr, "%d |\t Warning : Returned expression and return type are incompatible pointer types\n", line);
                                  }
                                }

                                if(!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                  if ($2->truelist || $2->falselist){
                                    qid tmp = newtmp("bool", level, level_id);
                                    if ($2->truelist && $2->falselist){
                                      int k0 = emit("=", NULL, NULL, tmp);
                                      patch_constant("1", k0);
                                      int k1 = emit("GOTO", NULL, NULL, NULL);
                                      int k2 = emit("=", NULL, NULL, tmp);
                                      patch_constant("0", k2);
                                      backpatch($2->truelist, k0);
                                      backpatch($2->falselist, k2);
                                      $2->nextlist = insert($2->nextlist, k1);
                                      backpatch($2->nextlist, nextinstr());
                                    }
                                    else if ($2->truelist){
                                      int k0 = emit("=", NULL, NULL, tmp);
                                      patch_constant("1", k0);
                                      backpatch($2->truelist, k0);
                                    }
                                    else if ($2->falselist){
                                      int k0 = emit("=", NULL, NULL, tmp);
                                      patch_constant("0", k0);
                                      backpatch($2->falselist, k0);
                                    }

                                    qid tmp1 = emit_assignment(type, "bool", tmp, level, level_id, line);
                                    int k = emit("RETURN", NULL, tmp1, NULL);
                                  }
                                  else {
                                    backpatch($2->nextlist, nextinstr());
                                    qid tmp = emit_assignment(type, $2->nodetype, $2->place, level, level_id, line);
                                    int k = emit("RETURN", NULL, tmp, NULL);
                                  }
                                }
                              }
	;

translation_unit
	: external_declaration                  {$$ = $1; }
	| translation_unit M external_declaration {$$ = non_terminal(0, "translation_unit", $1, $3);
                                              backpatch($1->nextlist, $2);
                                              $$->nextlist = copy($3->nextlist);
                                            }
	;

external_declaration
	: function_definition   {$$ = $1; t_name="";}
	| declaration           {$$ = $1; t_name="";}
	;

function_definition
	: declaration_specifiers declarator empty2 declaration_list M compound_statement empty3   {
                                                                                            $$ = non_terminal(1, "function_definition", $1, $2, $4, $6);
                                                                                            sym_tab_entry* entry = lookup_use($2->symbol, level, level_id);
                                                                                            if (entry){
                                                                                              entry->init = true;
                                                                                            }
                                                                                            t_name = "";
                                                                                            if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                                              string end = "*function end*\t(" + string($2->symbol) + ")";
                                                                                              int k = emit(end, NULL, NULL, NULL);
                                                                                              backpatch($4->nextlist, $5);
                                                                                              backpatch($6->nextlist, k);
                                                                                            }
                                                                                          }
	| declaration_specifiers declarator empty2 compound_statement empty3            {
                                                                                    $$ = non_terminal(1, "function_definition", $1, $2, $4);
                                                                                    sym_tab_entry* entry = lookup_use($2->symbol, level, level_id);
                                                                                    if (entry){
                                                                                      entry->init = true;
                                                                                    }
                                                                                    t_name = "";
                                                                                    if (!error_throw){ $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
                                                                                      string end = "*function end*\t(" + string($2->symbol) + ")";
                                                                                      int k = emit(end, NULL, NULL, NULL);
                                                                                      backpatch($4->nextlist, k);
                                                                                    }
                                                                                  }
	;

empty2
  : %empty{
          set_current_tab(func_name);
          t_name = "";
          string start = "*function start*\t(" + func_name + ")";
          emit(start, NULL, NULL, NULL);
        }
  ;

empty3
  :  %empty{
          set_current_tab("#");
          t_name = "";
        }
  ;

M
  : %empty {$$ = nextinstr();}
  ;

N
  : %empty  { $$ = terminal("N");
              $$->nextlist = NULL; $$->truelist = NULL; $$->falselist = NULL; $$->breaklist = NULL; $$->continuelist = NULL; $$->caselist = NULL; $$->place = NULL; $$->address = NULL;
              int k = emit("GOTO", NULL, NULL, NULL);
              ($$->nextlist) = insert($$->nextlist, k);
            }
  ;

%%

void yyerror(char* s){
    fprintf(stderr, "%s near line %d\n", s, line);
    exit(0);
}

void usage_info(){
    printf("Specify -help for usage info\n");
    printf("Specify test program with -i flag\n");
    printf("Provide -link flag to link library functions\n");
    return;
}

extern FILE* yyin;
int main (int argc, char* argv[]){
    if(argc == 1){
        usage_info();
        return 0;
    }
    bool link_lib_funcs = false;
    yyin = NULL;
    ast = NULL;
    for(int i = 0; i < argc-1; i++){
        if(!strcmp(argv[i+1], "-help")){
            usage_info();
            return 0;
        }
        if(!strcmp(argv[i+1], "-i")){
            yyin = fopen(argv[i+2], "r");
            i++;
        }
        else if(!strcmp(argv[i+1], "-link")){
            link_lib_funcs = true;
        }
        else{
            usage_info();
            return 0;
        }
    }
    if(yyin == NULL){
        printf("Please specify a valid input file\n");
        return 0;
    }
    ast = fopen("./out/tree.ast", "w");
    for (int i=0; i<MAX_LEVELS; i++) level_id[i] = 0;
    tab_init(link_lib_funcs);
    graph_init();
    yyparse();
    graph_end();
    if (error_throw){
      return 0;
    }
    sort_and_align_offsets();
    system("exec rm -r ./out/sym_tables/*");
    system("exec rm -r ./out/type_tables/*");
    dump_tables();
    dump_type_tables();
    dump_3ac();
    code_gen(link_lib_funcs);
    fclose (yyin);
    fclose (ast);
    return 0;
}