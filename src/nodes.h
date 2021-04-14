#include <bits/stdc++.h>

using namespace std;

typedef struct {
    int id;
    char* label;
    string nodetype;
    long long int int_val;
    long double float_val;
    bool bool_val;
    bool init = false;
    string symbol;
    int expr_type;
    unsigned long long size;
    unsigned long long max_size = 0;
} node;

typedef struct {
    int type; /* 0 int, 1 long, 2 long long, 3 float, 4 double, 5 long double */
    bool is_unsigned = false;
    long long int int_val;
    long double float_val;
} number;

void graph_init();
void graph_end();
node* terminal(char* label);
node* non_terminal(int case_no, char* label, node* n1 = NULL, node* n2 = NULL, node* n3 = NULL, node* n4 = NULL, node* n5 = NULL, char* op1 = NULL, char* op2 = NULL, char* op3 = NULL);