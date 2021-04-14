#include "sym_table.h"
#include "type_check.h"

map<string, sym_tab*> func_sym_tab_map;
map<string, string> func_args_map; 
sym_tab global_sym_tab;
sym_tab waste;
type_tab global_type_tab;
map<sym_tab*, long long> offsets;
sym_tab* curr;
// total 10 types : int, float, char, bool, ptr, struct, func, array, void, null

void tab_init(){
    curr = &global_sym_tab;
    offsets[curr] = 0;
    // add reserved keywords in table so they arent used again
}

void set_current_sym_tab(string func_name){
    if (func_name == "#"){
        curr = &global_sym_tab;
    }
    else {
        if (func_sym_tab_map.find(func_name) != func_sym_tab_map.end()) curr = func_sym_tab_map[func_name];
        else curr = &waste;
    }
}

int make_symbol_table(string func_name){
    if (global_sym_tab.find(make_tuple(func_name, 0, 0)) != global_sym_tab.end()){
        return -1;
    }
    sym_tab* new_tab = new sym_tab;
    func_sym_tab_map[func_name] = new_tab;
    offsets[new_tab] = 0;
    return 0;
}

void insert_entry(string sym_name, string type, unsigned long long size, long long offset, bool init, unsigned long long level, unsigned long long level_id){
    sym_tab_entry* entry = new sym_tab_entry();
    entry->sym_name = sym_name;
    entry->type = type;
    entry->size = size;
    entry->offset = offsets[curr];
    offsets[curr] += size;
    entry->init = init;
    entry->level = level;
    entry->level_id = level_id;
    (*curr)[make_tuple(entry->sym_name, entry->level, entry->level_id)] = entry;
}

sym_tab_entry* lookup_decl(string sym_name, unsigned long long level, unsigned long long level_id){
    if ((*curr).find(make_tuple(sym_name, level, level_id)) != (*curr).end()){
        return (*curr)[make_tuple(sym_name, level, level_id)];
    }
    return NULL;
}

sym_tab_entry* lookup_use(string sym_name, unsigned long long level, unsigned long long* level_id){
    while (level){
        if ((*curr).find(make_tuple(sym_name, level, level_id[level])) != (*curr).end()){
            return (*curr)[make_tuple(sym_name, level, level_id[level])];
        }
        level--;
    }
    if (global_sym_tab.find(make_tuple(sym_name, level, level_id[level])) != global_sym_tab.end()){
        return global_sym_tab[make_tuple(sym_name, level, level_id[level])];
    }
    return NULL;
}

void insert_type_entry(string type_name, string type, unsigned long long size, unsigned long long level, unsigned long long level_id){
    type_tab_entry* entry = new type_tab_entry();
    entry->type_name = type_name;
    entry->type = type;
    entry->size = size;
    entry->level = level;
    entry->level_id = level_id;
    global_type_tab[make_tuple(entry->type_name, entry->level, entry->level_id)] = entry;
}

type_tab_entry* lookup_type_decl(string type_name, unsigned long long level, unsigned long long level_id){
    if (global_type_tab.find(make_tuple(type_name, level, level_id)) != global_type_tab.end()){
        return global_type_tab[make_tuple(type_name, level, level_id)];
    }
    return NULL;
}

type_tab_entry* lookup_type_use(string type_name, unsigned long long level, unsigned long long* level_id){
    while (level){
        if (global_type_tab.find(make_tuple(type_name, level, level_id[level])) != global_type_tab.end()){
            return global_type_tab[make_tuple(type_name, level, level_id[level])];
        }
        level--;
    }
    if (global_type_tab.find(make_tuple(type_name, 0, level_id[0])) != global_type_tab.end()){
        return global_type_tab[make_tuple(type_name, 0, level_id[0])];
    }
    return NULL;
}

unsigned long long get_size(string type, unsigned long long level, unsigned long long* level_id){
    if(type == string("int")) return sizeof(int);
    if(type == string("long")) return sizeof(long);
    if(type == string("long int")) return sizeof(long int);
    if(type == string("long long")) return sizeof(long long);
    if(type == string("long long int")) return sizeof(long long int);
    if(type == string("unsigned int")) return sizeof(unsigned int);
    if(type == string("unsigned long int")) return sizeof(unsigned long int);
    if(type == string("unsigned long")) return sizeof(unsigned long);
    if(type == string("unsigned long long")) return sizeof(unsigned long long);
    if(type == string("unsigned long long int")) return sizeof(unsigned long long int);
    if(type == string("signed int")) return sizeof(signed int);
    if(type == string("signed long int")) return sizeof(signed long int);
    if(type == string("signed long")) return sizeof(signed long);
    if(type == string("signed long long")) return sizeof(signed long long);
    if(type == string("signed long long int")) return sizeof(signed long long int);
    if(type == string("short")) return sizeof(short);
    if(type == string("short int")) return sizeof(short int);
    if(type == string("unsigned short int")) return sizeof(unsigned short int);
    if(type == string("signed short int")) return sizeof(signed short int);
    if(type == string("unsigned short")) return sizeof(unsigned short);
    if(type == string("signed short")) return sizeof(signed short);

    if(type == string("float")) return sizeof(float);
    if(type == string("double")) return sizeof(double);
    if(type == string("long double")) return sizeof(long double);

    if(type == string("char")) return sizeof(char);

    if(type == string("bool")) return 1;

    if (type == string("void")) return 1;

    if (type[type.size()-1] == '*') return 8;

    if (lookup_type_use(type, level, level_id)) return lookup_type_use(type, level, level_id)->size;

    if (type == "null") return 0;

    return 8;
}

bool is_valid_type(string type, unsigned long long level, unsigned long long* level_id){
    if (is_type_int(type) || is_type_float(type) || type == "bool" || type == "char" || type == "void" || type[type.size()-1] == '*') return true;
    if (lookup_type_use(type, level, level_id)) return true;
    return false;
}

bool args_to_scope(string func_name, string func_args, string func_symbols, unsigned long long level, unsigned long long* level_id){
    set_current_sym_tab(func_name);
    string delim = ",";
    size_t f1 = 1;
    size_t f2 = 1;
    string temp1 = func_args;
    string temp2 = func_symbols;
    while(f1 != string::npos && f2 != string::npos){
        f1 = temp1.find_first_of(delim);
        f2 = temp2.find_first_of(delim);
        string type = temp1.substr(0, f1);
        temp1 = temp1.substr(f1+1);
        string symbol = temp2.substr(0, f2);
        temp2 = temp2.substr(f2+1);
        
        if (lookup_decl(symbol, level, level_id[level])){
            set_current_sym_tab("#");
            return false;   
        }
        else {
            insert_entry(symbol, type, get_size(type, level, level_id), 0, false, 1, 0);
        }
    }
    set_current_sym_tab("#");
    return true;
}

bool is_consistent(string func_name, string args){
    if (args == "") args = "null";
    // cout << "is cons : " << func_name << " : " << func_args_map[func_name] << endl;
    if (func_args_map[func_name] == args) return true;
    return false;
}

void insert_func_args(string func_name, string arg_types){
    // cout << "insert : " << func_name << " : " << arg_types << endl;
    func_args_map[func_name] = arg_types;
}

string get_func_args(string func_name){
    return func_args_map[func_name];
}

void update_func_type(string func_name, unsigned long long level, unsigned long long level_id, string type){
    if (func_sym_tab_map.find(func_name) != func_sym_tab_map.end()){
        string ins = "func " + type;
        ((*curr)[make_tuple(func_name, level, level_id)])->type = &ins[0];
    }
}

string get_func_ret_type(string func_name){
    sym_tab_entry* entry = global_sym_tab[make_tuple(func_name, 0, 0)];
    string type = entry->type;
    if (type.substr(0, 5) == "func "){
        type.erase(0, 5);
        return type;
    }
    return "null";
}

void dump_tables(){
    string glob = "../global_sym_table.csv";
    ofstream out_file(glob);
    for (auto itr : global_sym_tab){
        cout << itr.second->sym_name << "," << itr.second->type << "," << itr.second->size << "," << itr.second->offset << "," << itr.second->init << "," << itr.second->level << "," << itr.second->level_id << "\n";
    }
    cout << endl;
    for (auto table : func_sym_tab_map){
        string filename = "../" + table.first + "_table.csv";
        ofstream out_file(filename);
        for (auto itr : *(table.second)){
            cout << itr.second->sym_name << "," << itr.second->type << "," << itr.second->size << "," << itr.second->offset << "," << itr.second->init << "," << itr.second->level << "," << itr.second->level_id << "\n";
        }
        cout << endl;
    }
}