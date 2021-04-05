#include "type_check.h"

// set scope variables in sym table, we dont have to make a nester sym table
// sym table schema (id, name, size, offsets, init, scope_id, scope_offset)

bool is_type_int(string str){
    if (str == string("int") || str == string("long") || str == string("long int")) return true;
    if (str == string("long long") || str == string("long long int")) return true;
    if (str == string("unsigned int") || str == string("unsigned long") || str == string("unsigned long int")) return true;
    if (str == string("unsigned long long") || str == string("unsigned long long int")) return true;
    if (str == string("short") || str == string("short int")) return true;
    if (str == string("unsigned short") || str == string("unsigned short int")) return true;
    return false;
}

bool is_type_float(string str){
    if (str == string("float") || str == string("double") || str == string("long double")) return true;
    if (str == string("unsigned float") || str == string("unsigned double") || str == string("unsigned long double")) return true;
    return false;
}

string id_type(string str){
    sym_tab_entry* e = lookup(str);
    if (e) return e->type;
    return string("");
}

string const_type(string str){
    // write this function to check if a constant belongs to :
    // int, long, long long, float, double, long double
}

string postfix_type(string str, int label){
    string ret_type = string(str);
    if (label == 1){
        if (str[str.size()-1] == '*'){
            ret_type[ret_type.size()-1] = '\0';
            return ret_type;
        }
        else return string("");
    }
    else if (label == 2 || label == 3){

    }
    else if (label == 6 || label == 7){
        if (is_type_int(str)) return ret_type;
        else return string("");
    }
}

string args_type(string str1, string str2){
    if (str1 == string("void") && str2 == string("void")) return string("void");
    else return string("");
}

string unary_type(char* op, string str){
    string ret_type("");
    if (*op == '&'){
        ret_type += str + string("*");
    }
    else if (*op == '*'){
        return postfix_type(str, 1);
    }
    else if (*op == '+' || *op == '-'){
        if (is_type_int(str) || is_type_float(str)) ret_type += str;
    }
    else if (*op == '~'){
        if (is_type_int(str) || str == string("bool")) ret_type += str;
    }
    else if (*op == '!'){
        if (is_type_int(str) || is_type_float(str) || str == string("bool")) ret_type += str;
    }
    return ret_type;
}

string mul_type(string str1, string str2, char op){
    string ret_type = string("");
    if ((is_type_int(str1) || is_type_float(str1)) && (is_type_int(str2) || is_type_float(str2))){
        if (op == '*' || op == '/'){
            if (is_type_int(str1) && is_type_int(str2)) ret_type += "int";
            else ret_type += "float";
        }
        else if (op == '%'){
            if (is_type_int(str1) && is_type_int(str2)) ret_type += "int";
        }
    }
    return ret_type;
}

string add_type(string str1, string str2){
    string ret_type = string("");
    if ((is_type_int(str1) || is_type_float(str1)) && (is_type_int(str2) || is_type_float(str2))){
        if (is_type_int(str1) && is_type_int(str2)) ret_type += "int";
        else ret_type += "float";
    }
    else if ((str1 == string("char") && is_type_int(str2)) || (str2 == string("char") && is_type_int(str1))){
        ret_type += "int";
    }
    else if (str1[str1.size()-1] == '*' && is_type_int(str2)){
        ret_type += str1;
    }
    else if (str2[str2.size()-1] == '*' && is_type_int(str1)){
        ret_type += str2;
    }
    return ret_type;
}

string shift_type(string str1, string str2){
    if (is_type_int(str1) && is_type_int(str2)){
        return string(str1);
    }
    return string("");
}

string relat_type(string str1, string str2){
    string ret_type = string("");
    if ((is_type_int(str1) || is_type_float(str1) || str1 == string("char")) && (is_type_int(str2) || is_type_float(str2) || str2 == string("char"))){
        if ((is_type_int(str1) || str1 == string("char")) && (is_type_int(str2) || str2 == string("char"))) ret_type += "int";
        else ret_type += "float";
    }
    if (str1[str1.size()-1] == '*' && str2[str2.size()-1] == '*') ret_type += "*";
    return ret_type;
}

string bit_type(string str1, string str2){
    string ret_type = string("");
    if ((is_type_int(str1) || str1 == string("bool") || str1 == string("char")) && (is_type_int(str2) || str2 == string("bool") || str2 == string("char"))){
        if (str1 == string("bool") && str2 == string("bool")) ret_type += "bool";
        else ret_type += "int";
    }
    return ret_type;
}