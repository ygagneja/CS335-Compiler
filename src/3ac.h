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

extern vector <quad> code_arr;
extern unordered_map <string, int> user_goto;


string new_symbol();
qid newtmp(string type, unsigned long long level, unsigned long long* level_id);
qid newstring(string type, unsigned long long size, unsigned long long level, unsigned long long* level_id);
int emit(string op, qid op1, qid op2, qid res);
int nextinstr();
char* merge(char* l1, char* l2);
char* insert(char* li, int tmp);
char* copy(char* li);
void backpatch(char* li, int tmp);
void patch_constant(string constant, int addr);
void patch_caselist(char* li, qid arg1);
qid emit_assignment(string str1, string str2, qid place2, unsigned long long level, unsigned long long* level_id, int line);
void emit_assignment_multi(string op, string str1, string str2, qid place1, qid place2, qid address1, unsigned long long level, unsigned long long* level_id, int line);
void dump_3ac();