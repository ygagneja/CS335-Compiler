#include<bits/stdc++.h>
#include "sym_table.h"

using namespace std;

typedef sym_tab_entry* qid;

typedef struct clist{
    int val;
    struct clist* next = NULL;
}clist;

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
clist* merge(clist* l1, clist* l2);
clist* copy(clist* li);
clist* insert(clist* li, int tmp);
void backpatch(clist* li, int tmp);
bool insert_user_label(string label, int addr);
void patch_constant(string constant, int addr);
bool patch_user_goto(string label, int addr);
void patch_caselist(clist* li, qid arg1);
void emit_assignment(string str1, string str2, qid place1, qid place2, unsigned long long level, unsigned long long* level_id);
void emit_assignment_multi(string op, string str1, string str2, qid place1, qid place2, unsigned long long level, unsigned long long* level_id);
void dump_3ac();