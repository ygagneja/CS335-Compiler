#include "3ac.h"
#include "type_check.h"

long long counter = 0;

vector<quad> code_arr;

string new_symbol(){
    counter++;
    string new_sym = "_tmp_" + to_string(counter);
    return new_sym;
}

qid newtmp(string type, unsigned long long level, unsigned long long* level_id){
    string sym = new_symbol();
    return (qid)insert_entry(sym, type, get_size(type, level, level_id), 0, 1, level, level_id[level]);
}

qid newstring(string type, unsigned long long size, unsigned long long level, unsigned long long* level_id){
    string sym = new_symbol();
    return (qid)insert_entry(sym, type, size, 0, 1, level, level_id[level]);
}

int emit(string op, qid arg1, qid arg2, qid res){
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

char* merge(char* l1, char* l2){
    if (!l1 && !l2) return NULL;
    else if (l1 && !l2){
        return l1;
    }
    else if (!l1 && l2){
        return l2;
    }
    else {
        string ret = string(l1) + "," + string(l2);
        char* str = new char[ret.size()+1];
        strcpy(str, ret.c_str());
        return str;
    }
}

char* insert(char* li, int tmp){
    if (li == NULL){
        string str = to_string(tmp);
        li = new char[str.size()+1];
        strcpy(li, str.c_str());
        return li;
    }
    else {
        string str(li);
        str += "," + to_string(tmp);
        free(li);
        li = new char[str.size()+1];
        strcpy(li, str.c_str());
        return li;
    }
}

char* copy(char* li){
    if (li == NULL) return NULL;
    string str(li);
    char* ret = new char[str.size()+1];
    strcpy(ret, str.c_str());
    return ret;
}

void backpatch(char* li, int tmp){
    if (li == NULL) return;
    string str(li);
    string delim = ",";
    size_t f = 1;
    while (f != string::npos){
        f = str.find_first_of(delim);
        string t = str.substr(0, f);
        code_arr[stoi(t)].goto_label = tmp;
        if (f == string::npos) break;
        else str = str.substr(f+1);
    }
}

void patch_constant(string constant, int addr){
    code_arr[addr].constant = string(constant);
}

void patch_caselist(char* li, qid arg2){
    if (li == NULL) return;
    string str(li);
    string delim = ",";
    size_t f = 1;
    while (f != string::npos){
        f = str.find_first_of(delim);
        string t = str.substr(0, f);
        code_arr[stoi(t)].arg2 = arg2;
        if (f == string::npos) break;
        else str = str.substr(f+1);
    }
}

qid emit_assignment(string str1, string str2, qid place2, unsigned long long level, unsigned long long* level_id, int line){
    if (str1 == str2){
        return place2;
    }
    if ((is_type_int(str1) || is_type_float(str1) || is_type_char(str1) || is_type_bool(str1)) && (is_type_int(str2) || is_type_float(str2) || is_type_char(str2) || is_type_bool(str2))){
        // inttofloat inttobool inttochar, floattoint floattochar floattobool, chartofloat chartoint chartobool, booltofloat booltoint booltochar
        if (is_type_int(str2)){
            if (is_type_float(str1)){
                qid tmp = newtmp(str1, level, level_id);
                emit("inttofloat", NULL, place2, tmp);
                return tmp;
            }
            else if (is_type_bool(str1)){
                qid tmp = newtmp(str1, level, level_id);
                emit("inttobool", NULL, place2, tmp);
                return tmp;
            }
            else if (is_type_char(str1)){
                qid tmp = newtmp(str1, level, level_id);
                emit("inttochar", NULL, place2, tmp);
                return tmp;
            }
        }
        else if (is_type_float(str2)){
            if (is_type_int(str1)){
                qid tmp = newtmp(str1, level, level_id);
                emit("floattoint", NULL, place2, tmp);
                return tmp;
            }
            else if (is_type_bool(str1)){
                qid tmp = newtmp(str1, level, level_id);
                emit("floattobool", NULL, place2, tmp);
                return tmp;
            }
            else if (is_type_char(str1)){
                qid tmp = newtmp(str1, level, level_id);
                emit("floattochar", NULL, place2, tmp);
                fprintf(stderr, "%d |\t Warning : Converting float to char\n", line);
                return tmp;
            }
        }
        else if (is_type_bool(str2)){
            if (is_type_float(str1)){
                qid tmp = newtmp(str1, level, level_id);
                emit("booltofloat", NULL, place2, tmp);
                fprintf(stderr, "%d |\t Warning : Converting bool to float\n", line);
                return tmp;
            }
            else if (is_type_int(str1)){
                qid tmp = newtmp(str1, level, level_id);
                emit("booltoint", NULL, place2, tmp);
                return tmp;
            }
            else if (is_type_char(str1)){
                qid tmp = newtmp(str1, level, level_id);
                emit("booltochar", NULL, place2, tmp);
                fprintf(stderr, "%d |\t Warning : Converting bool to char\n", line);
                return tmp;
            }
        }
        else if (is_type_char(str2)){
            if (is_type_float(str1)){
                qid tmp = newtmp(str1, level, level_id);
                emit("chartofloat", NULL, place2, tmp);
                fprintf(stderr, "%d |\t Warning : Converting char to float\n", line);
                return tmp;
            }
            else if (is_type_bool(str1)){
                qid tmp = newtmp(str1, level, level_id);
                emit("chartobool", NULL, place2, tmp);
                fprintf(stderr, "%d |\t Warning : Converting char to bool\n", line);
                return tmp;
            }
            else if (is_type_int(str1)){
                qid tmp = newtmp(str1, level, level_id);
                emit("chartoint", NULL, place2, tmp);
                return tmp;
            }
        }
    }
    else if ((is_type_ptr(str1) && is_type_int(str2)) || (is_type_ptr(str2) && is_type_int(str1)) || is_type_ptr(str1) && is_type_ptr(str2)){
        qid tmp = newtmp(str1, level, level_id);
        emit("=", NULL, place2, tmp);
        return tmp;
    }
}

void emit_assignment_multi(string op, string str1, string str2, qid place1, qid place2, qid address1, unsigned long long level, unsigned long long* level_id, int line){
    if (op == "*=" || op == "/=" || op == "%="){
        op = op.substr(0, 1);
        string type = mul_type(str1, str2, op[0]);
        qid p1 = emit_assignment(type, str1, place1, level, level_id, line);
        qid p2 = emit_assignment(type, str2, place2, level, level_id, line);
        qid t = newtmp(type, level, level_id);
        emit(op+type, p1, p2, t);
        qid f = emit_assignment(str1, type, t, level, level_id, line);
        if (address1) emit("* =", NULL, f, address1);
        else emit("=", NULL, f, place1);
    }
    else if (op == "+=" || op == "-="){
        op = op.substr(0, 1);
        string type = add_type(str1, str2);
        if (type == "int" || type == "float"){
            qid p1 = emit_assignment(type, str1, place1, level, level_id, line);
            qid p2 = emit_assignment(type, str2, place2, level, level_id, line);
            qid t = newtmp(type, level, level_id);
            emit(op+type, p1, p2, t);
            qid f = emit_assignment(str1, type, t, level, level_id, line);
            if (address1) emit("* =", NULL, f, address1);
            else emit("=", NULL, f, place1);
        }
        else {
            if (is_type_int(str1)){
                qid res = newtmp(str1, level, level_id);
                qid tmp = newtmp(str1, level, level_id);
                string tp(str2); tp.pop_back();
                int k = emit("*int", place1, NULL, tmp);
                patch_constant(to_string(get_size(tp, level, level_id)), k);
                emit(op+"ptr", tmp, place2, res);
                if (address1) emit("* =", NULL, res, address1);
                else emit("=", NULL, res, place1);
            }
            else {
                qid res = newtmp(str1, level, level_id);
                qid tmp = newtmp(str2, level, level_id);
                string tp(str1); tp.pop_back();
                int k = emit("*int", place2, NULL, tmp);
                patch_constant(to_string(get_size(tp, level, level_id)), k);
                emit(op+"ptr", place1, tmp, res);
                if (address1) emit("* =", NULL, res, address1);
                else emit("=", NULL, res, place1);
            }
        }
    }
    else if (op == ">>=" || op == "<<="){
        op = op.substr(0, 2);
        string type = shift_type(str1, str2);
        qid p1 = emit_assignment(type, str1, place1, level, level_id, line);
        qid p2 = emit_assignment(type, str2, place2, level, level_id, line);
        qid t = newtmp(type, level, level_id);
        emit(op, p1, p2, t);
        qid f = emit_assignment(str1, type, t, level, level_id, line);
        if (address1) emit("* =", NULL, f, address1);
        else emit("=", NULL, f, place1);
    }
    else if (op == "&=" || op == "^=" || op == "|="){
        op = op.substr(0, 1);
        string type = bit_type(str1, str2);
        qid p1 = emit_assignment(type, str1, place1, level, level_id, line);
        qid p2 = emit_assignment(type, str2, place2, level, level_id, line);
        qid t = newtmp(type, level, level_id);
        emit(op, p1, p2, t);
        qid f = emit_assignment(str1, type, t, level, level_id, line);
        if (address1) emit("* =", NULL, f, address1);
        else emit("=", NULL, f, place1);
    }
}

void dump_3ac(){
    ofstream out_file("./out/out.3ac");
    int i = 0;
    for (quad q : code_arr){
        out_file << i << ".\t";
        cout << i << ".\t";
        if (q.res){
            out_file << q.res->sym_name;
            cout << q.res->sym_name;
        }
        else if (q.op == "GOTO" || q.op == "GOTO IF"){
            out_file << q.goto_label;
            cout << q.goto_label;
        }
        out_file << " <- ";
        cout << " <- ";
        if (q.arg1){
            out_file << q.arg1->sym_name;
            cout << q.arg1->sym_name;
        }
        out_file << " " << q.op << " ";
        cout << " " << q.op << " ";
        if (q.arg2){
            out_file << q.arg2->sym_name;
            cout << q.arg2->sym_name;
        }
        else {
            out_file << q.constant;
            cout << q.constant;
        }
        out_file << endl;
        cout << endl;
        i++;
    }
}