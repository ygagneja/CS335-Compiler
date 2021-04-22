#include "3ac.h"
#include "type_check.h"

long long counter = 0;

vector<quad> code_arr;
unordered_map<string, int> user_goto;

string new_symbol(){
    counter++;
    string new_sym = "#temp"+to_string(counter);
    return new_sym;
}

qid newtmp(string type, unsigned long long level, unsigned long long* level_id){
    string sym = new_symbol();
    insert_entry(sym, type, get_size(type, level, level_id), 0, 1, level, level_id[level]);
    qid ret = lookup_use(sym, level, level_id);
    return ret;
}


int emit(string op, qid arg1, qid arg2, qid res){
    quad tmp;
    tmp.op = op;
    tmp.arg1 = arg1;
    tmp.arg2 = arg2;
    tmp.res = res;
    code_arr.push_back(tmp);

    cout << code_arr.size()-1 << ".\t";
    if (tmp.res){
        cout << tmp.res->sym_name;
    }
    else if (tmp.op == "GOTO" || tmp.op == "GOTO IF"){
        cout << tmp.goto_label;
    }
    cout << " <- ";
    if (tmp.arg1){
        cout << tmp.arg1->sym_name;
    }
    cout << " " << tmp.op << " ";
    if (tmp.arg2){
        cout << tmp.arg2->sym_name;
    }
    else {
        cout << tmp.constant;
    }
    cout << endl;

    return code_arr.size()-1;
}

int nextinstr(){
    return code_arr.size();
}

clist* merge(clist* l1, clist* l2){
    clist* new1 = copy(l1);
    clist* new2 = copy(l2);
    if (!new1 && !new2) return NULL;
    else if (new1){
        clist* head = new1;
        while (new1->next != NULL) new1 = new1->next;
        new1->next = new2;
        return head;
    }
    else {
        clist* head = new2;
        while (new2->next != NULL) new2 = new2->next;
        new2->next = new1;
        return head;
    }
}

clist* copy(clist* li){
    clist* current = li;
    clist* newlist = NULL;
    clist* tail = NULL;
    while (current){
        if (newlist == NULL){
            newlist = (clist*)malloc(sizeof(clist));
            newlist->val = current->val;
            newlist->next = NULL;
            tail = newlist;
        }
        else {
            tail->next = (clist*)malloc(sizeof(clist));
            tail = tail->next;
            tail->val = current->val;
            tail->next = NULL;
        }
        current = current->next;
    }
    return newlist;
}

clist* insert(clist* li, int tmp){
    clist* head = (clist*)malloc(sizeof(clist));
    if (li == NULL){
        head->val = tmp; head->next = NULL;
        return head;
    }
    head->val = tmp;
    head->next = li;
    return head; 
}

void backpatch(clist* li, int tmp){
    while (li){
        code_arr[li->val].goto_label = tmp;
        li = li->next;
    }
}

bool insert_user_label(string label, int addr){
    if (user_goto.find(label) == user_goto.end()){
        user_goto[label] = addr;
        return true;
    }
    return false;
}

void patch_constant(string constant, int addr){
    code_arr[addr].constant = string(constant);
}

bool patch_user_goto(string label, int addr){
    if (user_goto.find(label) == user_goto.end()){
        return false;
    }
    code_arr[addr].goto_label = addr;
    return true;
}

void patch_caselist(clist* li, qid arg1){
    while (li){
        code_arr[li->val].arg1 = arg1;
        li = li->next;
    }
}

void emit_assignment(string str1, string str2, qid place1, qid place2, unsigned long long level, unsigned long long* level_id){
    if(is_type_int(str1) && is_type_int(str1)){
        emit("=", NULL, place2, place1);
	}
	else if(is_type_float(str1) && is_type_int(str2)){
		qid tmp = newtmp(str1, level, level_id);
		emit("inttofloat", place2, NULL, tmp);
		emit("=", NULL, tmp, place1);
	}
	else if(is_type_int(str1) && is_type_float(str2)){
		qid tmp = newtmp(str1, level, level_id);
		emit("floattoint", place2, NULL, tmp);
		emit("=", NULL, tmp, place1);
	}
	else if(is_type_float(str1) && is_type_float(str2)){
		emit("=", NULL, place2, place1);
	}
    else { // handles ptr to int and int to ptr and ptr to ptr
        emit("=", NULL, place2, place1);
    }
}

void emit_assignment_multi(string op, string str1, string str2, qid place1, qid place2, unsigned long long level, unsigned long long* level_id){
    if (op == "*=" || op == "/=" || op == "%="){
        op = op.substr(0, 1);
        if (is_type_int(str1) && is_type_int(str2)){
            emit(op + "int", place1, place2, place1);
        }   
        else if (is_type_int(str1) && is_type_float(str2)){
            qid tmp = newtmp(str2, level, level_id);
            emit("inttofloat", place1, NULL, tmp);
            emit(op + "float", tmp, place2, tmp);
            emit("floattoint", tmp, NULL, place1);
        }
        else if (is_type_float(str1) && is_type_int(str2)){
            qid tmp = newtmp(str1, level, level_id);
            emit("inttofloat", place2, NULL, tmp);
            emit(op + "float", place1, tmp, place2);
        }
        else {
            emit(op + "float", place1, place2, place1);
        }
    }
    else if (op == "+=" || op == "-="){
        op = op.substr(0, 1);
        string type = add_type(str1, str2);
        if (type == "int"){
            emit(op + "int", place1, place2, place1);
        }
        else if (type == "float"){
            if (is_type_int(str1)){
                qid tmp = newtmp(str2, level, level_id);
                emit("inttofloat", place1, NULL, tmp);
                emit(op + "float", tmp, place2, tmp);
                emit("floattoint", tmp, NULL, place1);
            }
            else if (is_type_int(str2)){
                qid tmp = newtmp(str1, level, level_id);
                emit("inttofloat", place2, NULL, tmp);
                emit(op + "float", place1, tmp, place2);
            }
            else {
                emit(op + "float", place1, place2, place1);
            }
        }
        else {
            if (is_type_int(str1)){
                string tp(str2); tp[tp.size()-1] = '\0';
                qid tmp = newtmp(str1, level, level_id);
                int k = emit("*int", place1, NULL, tmp);
                patch_constant(to_string(get_size(tp, level, level_id)), k);
                emit(op + "int", tmp, place2, place1);
            }
            else {
                string tp(str1); tp[tp.size()-1] = '\0';
                qid tmp = newtmp(str2, level, level_id);
                int k = emit("*int", place2, NULL, tmp);
                patch_constant(to_string(get_size(tp, level, level_id)), k);
                emit(op + "int", place1, tmp, place1);
            }
        }
    }
    else if (op == ">>=" || op == "<<="){
        op = op.substr(0, 2);
        emit(op, place1, place2, place1);
    }
    else if (op == "&=" || op == "^=" || op == "|="){
        op = op.substr(0, 1);
        emit(op, place1, place2, place1);
    }
}

void dump_3ac(){
    int i = 0;
    for (quad q : code_arr){
        cout << i << ".\t";
        if (q.res){
            cout << q.res->sym_name;
        }
        else if (q.op == "GOTO" || q.op == "GOTO IF"){
            cout << q.goto_label;
        }
        cout << " <- ";
        if (q.arg1){
            cout << q.arg1->sym_name;
        }
        cout << " " << q.op << " ";
        if (q.arg2){
            cout << q.arg2->sym_name;
        }
        else {
            cout << q.constant;
        }
        cout << endl;
        i++;
    }
    // handle goto label and constants
}