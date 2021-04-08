#include <bits/stdc++.h>

using namespace std;

typedef struct {
    int id;
    char* label;
    string nodetype;
    bool init = false;
    string symbol;
    int expr_type;
    unsigned long long size;
    unsigned long long max_size = 0;
} node;

void graph_init();
void graph_end();
node* terminal(char* label);
node* non_terminal(int case_no, char* label, node* n1 = NULL, node* n2 = NULL, node* n3 = NULL, node* n4 = NULL, node* n5 = NULL, char* op1 = NULL, char* op2 = NULL, char* op3 = NULL);