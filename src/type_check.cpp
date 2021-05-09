#include "type_check.h"
#include "sym_table.h"

// total 10 types : int, float, char, bool, ptr, struct, func, array, void, null

bool is_type_int(string str){
    if (str == string("int")) return true;
    return false;
}

bool is_type_float(string str){
    if (str == string("float")) return true;
    return false;
}

bool is_type_bool(string str){
    if (str == "bool") return true;
    return false;
}

bool is_type_char(string str){
    if (str == "char") return true;
    return false;
}

bool is_type_ptr(string str){
    if (str[str.size()-1] == '*') return true;
    return false;
}

bool is_type_func(string str){ 
    if (str.substr(0, 5) == "func ") return true;
    return false;
}

bool is_type_struct(string str){
    if (str.substr(0, 7) == "struct " && !is_type_ptr(str)) return true;
    return false;
}

bool is_type_structptr(string str){
    if (str.substr(0, 7) == "struct " && is_type_ptr(str)) return true;
    return false;
}

string is_valid(string str1, string str2){
    if (str1 == str2){
        if (is_type_func(str1) && is_type_struct(str1)) return "null";
        return "1";
    }
    if ((is_type_int(str1) || is_type_float(str1) || is_type_char(str1) || is_type_bool(str1)) && (is_type_int(str2) || is_type_float(str2) || is_type_char(str2) || is_type_bool(str2))){
        return string("1");
    }
    else if (is_type_ptr(str1) && is_type_int(str2)) return string("0");
    else if (is_type_ptr(str2) && is_type_int(str1)) return string("0");
    else if (is_type_ptr(str1) && is_type_ptr(str2)) return string("0");
    return string("null");
}

string id_type(string str, unsigned long long level, unsigned long long* level_id){
    sym_tab_entry* entry = lookup_use(str, level, level_id);
    if (entry) return entry->type;
    return string("null");
}

string postfix_type(string str, string exp_type, int label){
    string ret_type = string(str);
    if (label == 1){
        if (is_type_ptr(str) && is_type_int(exp_type)){
            ret_type.pop_back();
            return ret_type;
        }
        else return string("null");
    }
    else if (label == 2 || label == 3){
        if (is_type_func(str)){
            ret_type.erase(0, 5);
            return ret_type;
        }
        return string("null");
    }
    else if (label == 4){
        if (is_type_struct(str)){
            return "1";
        }
        return "null";
    }
    else if (label == 5){
        if (is_type_structptr(str)){
            str.pop_back();
            if (is_type_ptr(str)) return "null";
            return "1";
        }
        return "null";
    }
    else if (label == 6 || label == 7){
        if (is_type_int(str) || is_type_float(str) || is_type_char(str)) return ret_type;
        else return string("null");
    }
}

string args_type(string str1, string str2){
    if (str1 == string("void") && str2 != string("null")) return string("void");
    else return string("null");
}

string unary_type(char* op, string str){
    string ret_type("null");
    if (*op == '&' && !is_type_func(str)){
        return string(str + "*");
    }
    else if (*op == '*' && !is_type_func(str)){
        return postfix_type(str, "int", 1);
    }
    else if (*op == '+' || *op == '-'){
        if (is_type_int(str) || is_type_float(str)) return string(str);
        if (is_type_bool(str) || is_type_char(str)){
            if (*op == '+') return string(str);
            else return "int";
        }
    }
    else if (*op == '~'){
        if (is_type_int(str) || is_type_char(str) || is_type_bool(str)) return string(str);
    }
    else if (*op == '!'){
        if (is_type_int(str) || is_type_char(str) || is_type_bool(str) || is_type_float(str)) return string(str);
    }
    return ret_type;
}

string cast_type(string str1, string str2){
    return is_valid(str1, str2);
}

string mul_type(string str1, string str2, char op){
    string ret_type = string("null");
    if ((is_type_int(str1) || is_type_float(str1) || is_type_bool(str1) || is_type_char(str1)) && (is_type_int(str2) || is_type_float(str2) || is_type_bool(str2) || is_type_char(str2))){
        if (op == '*' || op == '/'){
            if (is_type_float(str1) || is_type_float(str2)) return "float";
            else return "int";
        }
        else if (op == '%'){
            if ((is_type_int(str1) || is_type_bool(str1) || is_type_char(str1)) && (is_type_int(str2) || is_type_bool(str2) || is_type_char(str2))) return string("int");
        }
    }
    return ret_type;
}

string add_type(string str1, string str2){
    string ret_type = string("null");
    if ((is_type_int(str1) || is_type_float(str1) || is_type_bool(str1) || is_type_char(str1)) && (is_type_int(str2) || is_type_float(str2) || is_type_bool(str2) || is_type_char(str2))){
        if (is_type_float(str1) || is_type_float(str2)) return "float";
        else return "int";
    }
    else if (is_type_ptr(str1) && is_type_int(str2)){
        return string(str1);
    }
    else if (is_type_ptr(str2) && is_type_int(str1)){
        return string(str2);
    }
    return ret_type;
}

string shift_type(string str1, string str2){
    if ((is_type_int(str1) || is_type_bool(str1) || is_type_char(str1)) && (is_type_int(str2) || is_type_bool(str2) || is_type_char(str2))){
        if (is_type_int(str1) || is_type_int(str2)) return "int";
        return "char";
    }
    return string("null");
}

string relat_type(string str1, string str2){
    string ret_type = string("null");
    if ((is_type_int(str1) || is_type_float(str1) || is_type_bool(str1) || is_type_char(str1)) && (is_type_int(str2) || is_type_float(str2) || is_type_bool(str2) || is_type_char(str2))){
        if (is_type_float(str1) || is_type_float(str2)) return "float";
        else return "int";
    }
    if (is_type_ptr(str1) && is_type_ptr(str2)){
        if (str1 == str2){
            return string("*");
        }
        else {
            return string("*warning");
        }
    }
    else if ((is_type_ptr(str1) && (is_type_int(str2))) || is_type_ptr(str2) && (is_type_int(str1))){
        return string("*warning");
    }
    return ret_type;
}

string bit_type(string str1, string str2){
    string ret_type = string("null");
    if ((is_type_int(str1) || is_type_char(str1) || is_type_bool(str1)) && (is_type_int(str2) || is_type_char(str2) || is_type_bool(str2))){
        if (is_type_int(str1) || is_type_int(str2)) return "int";
        return "char";
    }
    return ret_type;
}

string cond_type(string str1, string str2){
    if (str1 == str2){
        if (is_type_func(str1) && is_type_struct(str1)) return "null";
        return string(str1);
    }
    if ((is_type_int(str1) || is_type_float(str1) || is_type_char(str1) || is_type_bool(str1)) && (is_type_int(str2) || is_type_float(str2) || is_type_char(str2) || is_type_bool(str2))){
        if (is_type_float(str1) || is_type_float(str2)) return "float";
        if (is_type_int(str1) || is_type_int(str2)) return "int";
        return "char";
    }
    if (is_type_ptr(str1) && is_type_ptr(str2)) return string("void*");
    return string("null");
}

string assign_type(string str1, string str2, string op){
    if (op == "="){
        return is_valid(str1, str2);
    }
    else if (op == "*=" || op == "/=" || op == "%="){
        if (mul_type(str1, str2, op[0]) != "null") return string("1");
    }
    else if (op == "+=" || op == "-="){
        if (add_type(str1, str2) != "null") return string("1");
    }
    else if (op == ">>=" || op == "<<="){
        if (shift_type(str1, str2) != "null") return string("1");
    }
    else if (op == "&=" || op == "^=" || op == "|="){
        if (bit_type(str1, str2) != "null") return string("1");
    }
    return "null";
}