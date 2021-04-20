#include<bits/stdc++.h>
#include "sym_table.h"

using namespace std;

typedef pair<string, sym_tab_entry*> qid;

typedef struct{
    qid op;
    qid arg1;
    qid arg2;
    qid res;
} quad;

string new_symbol();
qid newtmp(string type, int level, int level_id);
int emit(qid op, qid op1, qid op2, qid res);