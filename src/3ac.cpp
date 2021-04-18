#include "3ac.h"

long long counter = 0;

vector<quad> code_arr;

// Function to return new symbol
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
    return 0; // Will modify if required
}