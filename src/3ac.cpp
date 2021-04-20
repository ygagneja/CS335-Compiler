#include "3ac.h"
#include "type_check.h"

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

void emit_assignment(string str1, string str2, qid place1, qid place2, unsigned long long level, unsigned long long* level_id){
    string op = "";
    if(is_type_int(str1) && is_type_int(str1)){
        emit({"=", NULL}, place2, {"", NULL}, place1);
	}
	else if(is_type_float(str1) && is_type_int(str2)){
		qid tmp = newtmp(str1, level, level_id);
		emit({"inttofloat",NULL}, place2, {"",NULL}, tmp);
		emit({"=", NULL}, tmp, {"", NULL}, place1)
	}
	else if(is_type_int(str1) && is_type_float(str2)){
		qid tmp = newtmp(str1, level, level_id);
		emit({"floattoint",NULL}, place2, {"",NULL}, tmp);
		emit({"=", NULL}, tmp, {"", NULL}, place1)
	}
	else if(is_type_float(str1) && is_type_float(str2)){
		emit({"=", NULL}, place2, {"", NULL}, place1);
	}
}