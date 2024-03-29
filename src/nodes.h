#include <bits/stdc++.h>
#include "3ac.h"

using namespace std;

typedef struct {
    int id;
    char* label;
    char* nodetype;
    long long int int_val;
    long double float_val;
    bool bool_val;
    bool init = false;
    char* symbol;
    char* curr_args_types;
    int expr_type;
    unsigned long long size;

    char* nextlist = NULL;
    char* truelist = NULL;
    char* falselist = NULL;
    char* breaklist = NULL;
    char* continuelist = NULL;
    char* caselist = NULL;
    qid place = NULL;
    qid address = NULL;
} node;

typedef struct {
    int int_val;
    float float_val;
    char* str;
} number;

void graph_init();
void graph_end();
node* terminal(char* label);
node* non_terminal(int case_no, char* label, node* n1 = NULL, node* n2 = NULL, node* n3 = NULL, node* n4 = NULL, node* n5 = NULL, char* op1 = NULL, char* op2 = NULL, char* op3 = NULL);