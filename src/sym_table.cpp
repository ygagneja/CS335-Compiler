#include "sym_table.h"
#include "type_check.h"

map<string, sym_tab*> func_sym_tab_map;
map<string, string> func_args_map;
map<string, vector<sym_tab_entry*>> func_syms_map;
map<string, long long> max_offsets;
sym_tab global_sym_tab;
sym_tab waste;
map<string, type_tab*> func_type_tab_map;
type_tab global_type_tab;
type_tab waste_type;
map<struct_sym_tab*, long long> offsets_struct;
sym_tab* curr;
type_tab* curr_type;
stack<struct_sym_tab*> curr_struct_stack;
// total 10 types : int, float, char, bool, ptr, struct, func, array, void, null

void tab_init(bool link_lib_funcs){
    curr = &global_sym_tab;
    curr_type = &global_type_tab;

    make_symbol_table("printi");
    make_symbol_table("printf");
    make_symbol_table("prints");
    make_symbol_table("scani");
    make_symbol_table("scanf");
    make_symbol_table("scans");
    make_symbol_table("malloc");
    args_to_scope("printi", "int", "x");
    args_to_scope("printf", "float", "x");
    args_to_scope("prints", "char*", "str");
    args_to_scope("scans", "char*,int", "str,len");
    args_to_scope("malloc", "int", "size");
    insert_entry("printi", "func void", 0, 0, true, 0, 0);
    insert_entry("printf", "func void", 0, 0, true, 0, 0);
    insert_entry("prints", "func void", 0, 0, true, 0, 0);
    insert_entry("scani", "func int", 0, 0, true, 0, 0);
    insert_entry("scanf", "func float", 0, 0, true, 0, 0);
    insert_entry("scans", "func void", 0, 0, true, 0, 0);
    insert_entry("malloc", "func void*", 0, 0, true, 0, 0);
    insert_func_args("printi", "int");
    insert_func_args("printf", "float");
    insert_func_args("prints", "char*");
    insert_func_args("scani", "null");
    insert_func_args("scanf", "null");
    insert_func_args("scans", "char*,int");
    insert_func_args("malloc", "int");
    if (link_lib_funcs){
        make_symbol_table("strlen");
        make_symbol_table("isqrt");
        make_symbol_table("log10");
        make_symbol_table("floor");
        make_symbol_table("power");
        make_symbol_table("sin");
        make_symbol_table("cos");
        args_to_scope("strlen", "char*", "str");
        args_to_scope("isqrt", "int", "x");
        args_to_scope("log10", "float", "x");
        args_to_scope("floor", "float", "x");
        args_to_scope("power", "int,int", "x,y");
        args_to_scope("sin", "float", "x");
        args_to_scope("cos", "float", "x");
        insert_entry("strlen", "func int", 0, 0, true, 0, 0);
        insert_entry("isqrt", "func int", 0, 0, true, 0, 0);
        insert_entry("log10", "func float", 0, 0, true, 0, 0);
        insert_entry("floor", "func float", 0, 0, true, 0, 0);
        insert_entry("power", "func int", 0, 0, true, 0, 0);
        insert_entry("sin", "func float", 0, 0, true, 0, 0);
        insert_entry("cos", "func float", 0, 0, true, 0, 0);
        insert_func_args("strlen", "char*");
        insert_func_args("isqrt", "int");
        insert_func_args("log10", "float");
        insert_func_args("floor", "float");
        insert_func_args("power", "int,int");
        insert_func_args("sin", "float");
        insert_func_args("cos", "float");
    }
}

void set_current_tab(string func_name){
    if (func_name == "#"){
        curr = &global_sym_tab;
        curr_type = &global_type_tab;
    }
    else {
        if (func_sym_tab_map.find(func_name) != func_sym_tab_map.end()){
            curr = func_sym_tab_map[func_name];
            curr_type = func_type_tab_map[func_name];
        } 
        else {
            curr = &waste;
            curr_type = &waste_type;
        }
    }
}

int make_symbol_table(string func_name){
    if (global_sym_tab.find(make_tuple(func_name, 0, 0)) != global_sym_tab.end()){
        return -1;
    }
    type_tab* new_type_tab = new type_tab;
    func_type_tab_map[func_name] = new_type_tab;
    sym_tab* new_tab = new sym_tab;
    func_sym_tab_map[func_name] = new_tab;
    return 0;
}

sym_tab_entry* insert_entry(string sym_name, string type, unsigned long long size, long long offset, bool init, unsigned long long level, unsigned long long level_id){
    sym_tab_entry* entry = new sym_tab_entry();
    entry->sym_name = sym_name;
    entry->type = type;
    entry->size = size;
    entry->init = init;
    entry->level = level;
    entry->level_id = level_id;
    (*curr)[make_tuple(entry->sym_name, entry->level, entry->level_id)] = entry;
    return entry;
}

void sort_and_align_offsets(){
    for (auto table : func_sym_tab_map){
        long long offset = 0;
        for (auto itr : *(table.second)){
            sym_tab_entry* entry = itr.second;
            if (entry->size < 4){
                offset = offset + entry->size;
                entry->offset = offset;
            }
        }
        offset = offset % 4 ? offset + (4 - offset % 4) : offset;
        for (auto itr : *(table.second)){
            sym_tab_entry* entry = itr.second;
            if (entry->size == 4){
                offset = offset + entry->size;
                entry->offset = offset;
            }
        }
        for (auto itr : *(table.second)){
            sym_tab_entry* entry = itr.second;
            if (entry->size > 4){
                offset = entry->size % 4 ? offset + (4 - entry->size % 4) + entry->size : offset + entry->size;;
                entry->offset = offset;
            }            
        }
        max_offsets[table.first] = offset + 16;
    }
}

void set_arr_flag(string sym_name, unsigned long long level, unsigned long long level_id){
    (*curr)[make_tuple(sym_name, level, level_id)]->is_arr = true;
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

void set_struct_scope(){
    struct_sym_tab* tab = new struct_sym_tab;
    offsets_struct[tab] = 0;
    curr_struct_stack.push(tab);
}

bool insert_struct_symbol(string sym_name, string type, unsigned long long size){
    struct_sym_tab* curr;
    if (curr_struct_stack.size()) curr = curr_struct_stack.top();
    else return true;
    if ((*curr).find(sym_name) == (*curr).end()){
        struct_sym_entry* entry = new struct_sym_entry;
        entry->sym_name = sym_name;
        entry->type = type;
        entry->size = size;
        entry->offset = offsets_struct[curr];
        offsets_struct[curr] = offsets_struct[curr]%4 && size >= 4 ? offsets_struct[curr] + (4 - offsets_struct[curr]%4) + size : offsets_struct[curr] + size;
        (*curr)[sym_name] = entry;
        return true;
    }
    return false;
}

void set_struct_arr_flag(string sym_name){
    struct_sym_tab* curr;
    if (curr_struct_stack.size()) curr = curr_struct_stack.top();
    else return;
    if ((*curr).find(sym_name) != (*curr).end()){
        (*curr)[sym_name]->is_arr = true;
    }
}

void insert_type_entry(string type_name, unsigned long long size, unsigned long long level, unsigned long long level_id){
    type_tab_entry* entry = new type_tab_entry();
    entry->type_name = type_name;
    entry->size = size;
    entry->level = level;
    entry->level_id = level_id;
    if (curr_struct_stack.size()){
        entry->symbols = curr_struct_stack.top();
        curr_struct_stack.pop();
    }
    (*curr_type)[make_tuple(entry->type_name, entry->level, entry->level_id)] = entry;
}

type_tab_entry* lookup_type_decl(string type_name, unsigned long long level, unsigned long long level_id){
    if ((*curr_type).find(make_tuple(type_name, level, level_id)) != (*curr_type).end()){
        return (*curr_type)[make_tuple(type_name, level, level_id)];
    }
    return NULL;
}

type_tab_entry* lookup_type_use(string type_name, unsigned long long level, unsigned long long* level_id){
    while (level){
        if ((*curr_type).find(make_tuple(type_name, level, level_id[level])) != (*curr_type).end()){
            return (*curr_type)[make_tuple(type_name, level, level_id[level])];
        }
        level--;
    }
    if (global_type_tab.find(make_tuple(type_name, 0, level_id[0])) != global_type_tab.end()){
        return global_type_tab[make_tuple(type_name, 0, level_id[0])];
    }
    return NULL;
}


string struct_sym_type(string type, string sym_name, unsigned long long level, unsigned long long* level_id){
    type_tab_entry* entry = lookup_type_use(type, level, level_id);
    struct_sym_tab* tmp;
    if (entry && entry->symbols) tmp = entry->symbols;
    else return "null";

    if ((*tmp).find(sym_name) == (*tmp).end()) return "null";
    return (*tmp)[sym_name]->type;
}

bool is_struct_sym_arr(string type, string sym_name, unsigned long long level, unsigned long long* level_id){
    type_tab_entry* entry = lookup_type_use(type, level, level_id);
    struct_sym_tab* tmp;
    if (entry && entry->symbols) tmp = entry->symbols;
    else return false;

    if ((*tmp).find(sym_name) == (*tmp).end()) return false;
    return (*tmp)[sym_name]->is_arr;
}

long long get_struct_sym_offset(string type, string sym_name, unsigned long long level, unsigned long long* level_id){
    type_tab_entry* entry = lookup_type_use(type, level, level_id);
    struct_sym_tab* tmp;
    if (entry && entry->symbols) tmp = entry->symbols;
    else return 0;
    return (*tmp)[sym_name]->offset;
}

unsigned long long get_size(string type, unsigned long long level, unsigned long long* level_id){
    if(type == "int") return 4;
    if(type == "float") return 4;
    if(type == "char") return 1;
    if(type == "bool") return 1;
    if (type == "void") return 1;
    if (type[type.length()-1] == '*') return 4;
    if (level_id && lookup_type_use(type, level, level_id)) return lookup_type_use(type, level, level_id)->size;
    if (type == "null") return 0;
    return 4;
}

bool is_valid_type(string type, unsigned long long level, unsigned long long* level_id){
    while (is_type_ptr(type)) type.pop_back();
    if (is_type_int(type) || is_type_float(type) || is_type_char(type) || is_type_bool(type) || type == "void") return true;
    if (lookup_type_use(type, level, level_id)) return true;
    return false;
}

bool args_to_scope(string func_name, string func_args, string func_symbols){
    set_current_tab(func_name);
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
        
        if (lookup_decl(symbol, 1, 0)){
            set_current_tab("#");
            return false;   
        }
        else {
            sym_tab_entry* entry = insert_entry(symbol, type, get_size(type, 0, NULL), 0, false, 1, 0);
            func_syms_map[func_name].push_back(entry);
        }
    }
    set_current_tab("#");
    return true;
}

bool is_consistent(string func_name, string args){
    if (args == "") args = "null";
    if (func_args_map[func_name] == args) return true;
    return false;
}

bool check_args_constraints(string func_name, string args){
    string delim = ",";
    size_t f = 1;
    string temp = args;
    int f_args = 0, non_f_args = 0;
    while(f != string::npos){
        f = temp.find_first_of(delim);
        string type = temp.substr(0, f);
        temp = temp.substr(f+1);
        
        if (is_type_float(type)) f_args++;
        else non_f_args++;
    }
    if (f_args <= 4 && non_f_args <= 4) return true;
    else return false;
}

void insert_func_args(string func_name, string arg_types){
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
    cout << "\nGLOBAL SYMBOL TABLE\n";
    // string glob = "global_sym_table.csv";
    // ofstream out_file(glob);
    cout << "sym_name,type,size,offset,init,level,level_id\n";
    for (auto itr : global_sym_tab){
        cout << itr.second->sym_name << "," << itr.second->type << "," << itr.second->size << "," << itr.second->offset << "," << itr.second->init << "," << itr.second->level << "," << itr.second->level_id << "\n";
    }
    cout << endl;
    for (auto table : func_sym_tab_map){
        cout << "\nFUNCTION : " << table.first << " SYMBOL TABLE\n";
        // string filename = table.first + "_table.csv";
        // ofstream out_file(filename);
        cout << "sym_name,type,size,offset,init,level,level_id\n";
        for (auto itr : *(table.second)){
            cout << itr.second->sym_name << "," << itr.second->type << "," << itr.second->size << "," << itr.second->offset << "," << itr.second->init << "," << itr.second->level << "," << itr.second->level_id << "\n";
        }
        cout << endl;
    }
}

void dump_type_tables(){
    cout << "\nGLOBAL TYPE TABLE\n";
    cout << "sym_name,size,level,level_id\n";
    for (auto itr : global_type_tab){
        cout << itr.second->type_name  << "," << itr.second->size << "," << itr.second->level << "," << itr.second->level_id << "\n";
    }
    cout << endl;
    for (auto table : func_type_tab_map){
        cout << "\nFUNCTION : " << table.first << " TYPE TABLE\n";
        cout << "sym_name,size,level,level_id\n";
        for (auto itr : *(table.second)){
            cout << itr.second->type_name  << "," << itr.second->size << "," << itr.second->level << "," << itr.second->level_id << "\n";
        }
        cout << endl;
    }
}