#include "sym_table.h"
#include "type_check.h"

map<string, sym_tab*> func_sym_tab_map;
map<string, string> func_args_map; 
sym_tab global_sym_tab;
sym_tab waste;
map<string, type_tab*> func_type_tab_map;
type_tab global_type_tab;
type_tab waste_type;
map<sym_tab*, long long> offsets;
map<struct_sym_tab*, long long> offsets_struct;
sym_tab* curr;
type_tab* curr_type;
stack<struct_sym_tab*> curr_struct_stack;
// total 10 types : int, float, char, bool, ptr, struct, func, array, void, null

void tab_init(){
    curr = &global_sym_tab;
    curr_type = &global_type_tab;
    offsets[curr] = 0;
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
    offsets[new_tab] = 0;
    return 0;
}

void restore_offset(string type, unsigned long long level, unsigned long long* level_id){
    offsets[curr] -= get_size(type, level, level_id);
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
        offsets_struct[curr] += size;
        (*curr)[sym_name] = entry;
        return true;
    }
    return false;
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

unsigned long long get_size(string type, unsigned long long level, unsigned long long* level_id){
    if(type == "int") return 4;
    if(type == "float") return 4;
    if(type == "char") return 1;
    if(type == "bool") return 1;
    if (type == "void") return 1;
    if (type[type.length()-1] == '*') return 4;
    if (lookup_type_use(type, level, level_id)) return lookup_type_use(type, level, level_id)->size;
    if (type == "null") return 0;
    return 4;
}

bool is_valid_type(string type, unsigned long long level, unsigned long long* level_id){
    while (is_type_ptr(type)) type.pop_back();
    if (is_type_int(type) || is_type_float(type) || is_type_char(type) || is_type_bool(type) || type == "void") return true;
    if (lookup_type_use(type, level, level_id)) return true;
    return false;
}

bool args_to_scope(string func_name, string func_args, string func_symbols, unsigned long long level, unsigned long long* level_id){
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
        
        if (lookup_decl(symbol, level, level_id[level])){
            set_current_tab("#");
            return false;   
        }
        else {
            insert_entry(symbol, type, get_size(type, level, level_id), 0, false, 1, 0);
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