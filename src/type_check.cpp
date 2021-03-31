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

string id_type(string str){
    sym_tab_entry* e = lookup(str);
    if (e) return e->type;
    return "";
}

string const_type(string str){
    // write this function to check if a constant belongs to :
    // int, long, long long, float, double, long double
}

string postfix_type(string str, int label){
    string ret_type = string(str);
    if (label == 1){
        if (str.back() == '*'){
            ret_type.pop_back();
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