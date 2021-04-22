#include<bits/stdc++.h>
#include "sym_table.h"

using namespace std;

typedef sym_tab_entry* qid;

typedef struct{
    string op;
    qid arg1;
    qid arg2;
    qid res;
    int goto_label; // should be in arg2
    string constant; // should be in arg2
} quad;

string new_symbol();
qid newtmp(string type, unsigned long long level, unsigned long long* level_id);
int emit(string op, qid op1, qid op2, qid res);
int nextinstr();
vector<int> merge(vector<int> l1, vector<int> l2);
vector<int> copy(vector<int> li);
void backpatch(vector<int> li, int tmp);
bool insert_user_label(string label, int addr);
void patch_constant(string constant, int addr);
bool patch_user_goto(string label, int addr);
void patch_caselist(vector<int> li, qid arg1);
void emit_assignment(string str1, string str2, qid place1, qid place2, unsigned long long level, unsigned long long* level_id);
void emit_assignment_multi(string op, string str1, string str2, qid place1, qid place2, unsigned long long level, unsigned long long* level_id);
void dump_3ac();