#include "3ac.h"

long long counter = 0;

vector<quad> code_arr;
unordered_map<string, int> user_goto;

string new_symbol(){
    counter++;
    string new_sym = "__T"+to_string(counter)+"__";
    return new_sym;
}

qid newtmp(string type, unsigned long long level, unsigned long long* level_id){
    string sym = new_symbol();
    insert_entry(sym, type, get_size(type, level, level_id), 0, 1, level, level_id[level]);
    return {sym, lookup_use(sym, level, level_id)};
}


int emit(qid op, qid arg1, qid arg2, qid res){
    quad tmp;
    tmp.op = op;
    tmp.arg1 = arg1;
    tmp.arg2 = arg2;
    tmp.res = res;
    code_arr.push_back(tmp);
    return code_arr.size()-1;
}

int nextinstr(){
    return code_arr.size();
}

vector<int> merge(vector<int> l1, vector<int> l2){
    vector<int> tmp;
    for(int i: l1) tmp.push_back(i);
    for(int i: l2) tmp.push_back(i);
    return tmp;
}

void backpatch(vector<int> li, int tmp){
    for(int i: li){
        code_arr[i].goto_label = tmp;
    }
}

bool insert_user_label(string label, int addr){
    if (user_goto.find(label) == user_goto.end()){
        user_goto[label] = addr;
        return true;
    }
    return false;
}

bool patch_user_goto(string label, int addr){
    if (user_goto.find(label) == user_goto.end()){
        return false;
    }
    code_arr[addr].goto_label = addr;
    return true;
}

void patch_caselist(vector<int> li, qid arg1){
    for(int i: li){
        code_arr[i].arg1 = arg1;
    }
}