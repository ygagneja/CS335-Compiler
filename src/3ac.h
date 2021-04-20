#include<bits/stdc++.h>
#include "sym_table.h"

using namespace std;

typedef pair<string, sym_tab_entry*> qid;

typedef struct{
    qid op;
    qid arg1;
    qid arg2;
    qid res;
    int goto_label;
} quad;

string new_symbol();
qid newtmp(string type, unsigned long long level, unsigned long long* level_id);
int emit(qid op, qid op1, qid op2, qid res);
int nextinstr();
vector<int> merge(vector<int> l1, vector<int> l2);
void backpatch(vector<int> li, int tmp);
bool insert_user_label(string label, int addr);
bool patch_user_goto(string label, int addr);
void patch_caselist(vector<int> li, qid arg1);