#include "type_check.h"
#include "sym_table.h"

// set scope variables in sym table, we dont have to make a nester sym table
// sym table schema (id, name, size, offsets, init, scope_id, scope_offset)

// total 10 types : int, float, char, bool, ptr, struct, func, array, void, null

bool is_type_int(string str){
    if (str == string("int") || str == string("long") || str == string("long int")) return true;
    if (str == string("long long") || str == string("long long int")) return true;
    if (str == string("unsigned int") || str == string("unsigned long") || str == string("unsigned long int")) return true;
    if (str == string("unsigned long long") || str == string("unsigned long long int")) return true;
    if (str == string("signed int") || str == string("signed long") || str == string("signed long int")) return true;
    if (str == string("signed long long") || str == string("signed long long int")) return true;
    if (str == string("short") || str == string("short int")) return true;
    if (str == string("unsigned short") || str == string("unsigned short int")) return true;
    if (str == string("signed short") || str == string("signed short int")) return true;
    return false;
}

bool is_type_float(string str){
    if (str == string("float") || str == string("double") || str == string("long double")) return true;
    return false;
}

string is_valid(string str1, string str2){
    if (str1 == str2) return string("1");
    if ((is_type_int(str1) || is_type_float(str1) || str1 == string("char") || str1 == string("bool")) && (is_type_int(str2) || is_type_float(str2) || str2 == string("char") || str2 == string("bool"))){
        if (str1 == string("bool") || str2 == string("bool")) return string("null");
        if (is_type_int(str1) || is_type_float(str1) || str1 == string("char")) return string("1");
    }
    else if (str1[str1.size()-1] == '*' && (is_type_int(str2) || str2 == string("char"))) return string("0");
    else if (str2[str2.size()-1] == '*' && (is_type_int(str1) || str1 == string("char"))) return string("0");
    else if (str1[str1.size()-1] == '*' && str2[str2.size()-1] == '*'){
        if (str1 == string("void*") || str2 == string("void*") && str1 == str2) return string("1");
        else return string("0");
    }  
    return string("null");
}

string id_type(string str, unsigned long long level, unsigned long long* level_id){
    sym_tab_entry* entry = lookup_use(str, level, level_id);
    if (entry) return entry->type;
    return string("null");
}

string const_type(int label){
    if (label == 0) return "int";
    else if (label == 1) return "long";
    else if (label == 2) return "long long";
    else if (label == 3) return "float";
    else if (label == 4) return "double";
    else if (label == 5) return "long double";
}

string postfix_type(string str, int label){
    string ret_type = string(str);
    if (label == 1){
        if (str[str.size()-1] == '*'){
            ret_type[ret_type.size()-1] = '\0';
            return ret_type;
        }
        else return string("null");
    }
    else if (label == 2 || label == 3){
        if (str.substr(0, 5) == "func "){
            ret_type.erase(0, 5);
            return ret_type;
        }
        return string("null");
    }
    else if (label == 6 || label == 7){
        if (is_type_int(str) || is_type_float(str) || str == string("char") || str[str.size()-1] == '*') return ret_type;
        else return string("null");
    }
}

string args_type(string str1, string str2){
    if (str1 == string("void") && str2 != string("null")) return string("void");
    else return string("null");
}

string unary_type(char* op, string str){
    string ret_type("null");
    if (*op == '&'){
        return string(str + "*");
    }
    else if (*op == '*'){
        return postfix_type(str, 1);
    }
    else if (*op == '+' || *op == '-'){
        if (is_type_int(str) || is_type_float(str) || str == string("char")) return string(str);
    }
    else if (*op == '~'){
        if (is_type_int(str) || str == string("char") || str == string("bool")) return string(str);
    }
    else if (*op == '!'){
        if (is_type_int(str) || is_type_float(str) || str == string("char") || str == string("bool")) return string(str);
    }
    return ret_type;
}

string cast_type(string str1, string str2){
    return is_valid(str1, str2);
}

string mul_type(string str1, string str2, char op){
    string ret_type = string("null");
    if ((is_type_int(str1) || is_type_float(str1) || str1 == string("char")) && (is_type_int(str2) || is_type_float(str2) || str2 == string("char"))){
        if (op == '*' || op == '/'){
            if ((is_type_int(str1) || str1 == string("char")) && (is_type_int(str2) || str2 == string("char"))) return string("int");
            else return string("float");
        }
        else if (op == '%'){
            if ((is_type_int(str1) || str1 == string("char")) && (is_type_int(str2) || str2 == string("char"))) return string("int");
        }
    }
    return ret_type;
}

string add_type(string str1, string str2){
    string ret_type = string("null");
    if ((is_type_int(str1) || is_type_float(str1) || str1 == string("char")) && (is_type_int(str2) || is_type_float(str2) || str2 == string("char"))){
        if ((is_type_int(str1) || str1 == string("char")) && (is_type_int(str2) || str2 == string("char"))) return string("int");
        else return string("float");
    }
    else if (str1[str1.size()-1] == '*' && (is_type_int(str2) || str2== string("char"))){
        return string(str1);
    }
    else if (str2[str2.size()-1] == '*' && (is_type_int(str1) || str1 == string("char"))){
        return string(str2);
    }
    return ret_type;
}

string shift_type(string str1, string str2){
    if ((is_type_int(str1) || str1 == string("char")) && (is_type_int(str2) || str2 == string("char"))){
        return string(str1);
    }
    return string("null");
}

string relat_type(string str1, string str2){
    string ret_type = string("null");
    if ((is_type_int(str1) || is_type_float(str1) || str1 == string("char")) && (is_type_int(str2) || is_type_float(str2) || str2 == string("char"))){
        if ((is_type_int(str1) || str1 == string("char")) && (is_type_int(str2) || str2 == string("char"))) return string("int");
        else return string("float");
    }
    if (str1[str1.size()-1] == '*' && str2[str2.size()-1] == '*'){
        if (str1 == str2){
            return string("*");
        }
        else {
            return string("*warning");
        }
    }
    else if ((str1[str1.size()-1] == '*' && (is_type_int(str2) || str2 == string("char"))) || (str2[str2.size()-1] == '*') && (is_type_int(str1) || str1 == string("char"))){
        return string("*warning");
    }
    return ret_type;
}

string bit_type(string str1, string str2){
    string ret_type = string("null");
    if ((is_type_int(str1) || str1 == string("bool") || str1 == string("char")) && (is_type_int(str2) || str2 == string("bool") || str2 == string("char"))){
        if (str1 == string("bool") && str2 == string("bool")) return string("bool");
        else return string("int");
    }
    return ret_type;
}

string cond_type(string str1, string str2){
    if (str1 == str2) return string(str1);
    if ((is_type_int(str1) || is_type_float(str1) || str1 == string("char") || str1 == string("bool")) && (is_type_int(str2) || is_type_float(str2) || str2 == string("char") || str2 == string("bool"))){
        if (str1 == string("bool") || str2 == string("bool")) return string("bool");
        return string("long double");
    }
    if (str1[str1.size()-1] == '*' && str2[str2.size()-1] == '*') return string("void*");
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