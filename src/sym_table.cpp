#include "sym_table.h"

map<string, sym_tab*> func_sym_tab_map;
map<string, string> func_args; 
sym_tab global_sym_tab;
type_tab global_type_tab;
sym_tab* curr;
// total 9 types : int, float, char, bool, ptr, struct, func, array, void

void tab_init(){

    curr = &global_sym_tab;
    // add reserved keywords in table so they arent used again
}

void set_current_sym_tab(string func_name){
    if (func_name == ""){
        curr = &global_sym_tab;
    }
    else {
        curr = func_sym_tab_map[func_name];
    }
}

int make_symbol_table(string func_name){
    if (func_sym_tab_map.find(func_name) != func_sym_tab_map.end()){
        return -1;
    }
    sym_tab* new_tab = new sym_tab;
    func_sym_tab_map[func_name] = new_tab;
    return 0;
}

void insert_entry(string sym_name, string type, unsigned long long size, long long offset, bool init, unsigned long long level, unsigned long long level_id){
    sym_tab_entry* entry = new sym_tab_entry();
    entry->sym_name = sym_name;
    entry->type = type;
    entry->size = size;
    entry->offset = offset;
    entry->init = init;
    entry->level = level;
    entry->level_id = level_id;
    (*curr)[make_tuple(entry->sym_name, entry->level, entry->level_id)] = entry;
}

sym_tab_entry* lookup(string sym_name, unsigned long long level, unsigned long long level_id){
    if ((*curr).find(make_tuple(sym_name, level, level_id)) != (*curr).end()){
        return (*curr)[make_tuple(sym_name, level, level_id)];
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

type_tab_entry* lookup_type(string type_name){
    for (auto itr : global_type_tab){
        if (get<0>(itr.first) == type_name) return itr.second;
    }
    return NULL;
}

unsigned long long get_size(string type){
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

    if (type[type.size()-1] == '*') return 8;

    if (lookup_type(type)) return lookup_type(type)->size;

    if (type == "") return 0;

    return 8;
}

void insert_func_args(string func_name, string arg_types){
    func_args[func_name] = arg_types;
}

string get_func_args(string func_name){
    return func_args[func_name];
}

void dump_tables(){
    string glob = "../global_sym_table.csv";
    ofstream out_file(glob);
    for (auto itr : global_sym_tab){
        out_file << itr.second->sym_name << "," << itr.second->size << "," << itr.second->offset << "," << itr.second->init << "," << itr.second->level << "," << itr.second->level_id << "\n";
    }
    for (auto table : func_sym_tab_map){
        string filename = "../" + table.first + "_table.csv";
        ofstream out_file(filename);
        for (auto itr : *(table.second)){
            out_file << itr.second->sym_name << "," << itr.second->size << "," << itr.second->offset << "," << itr.second->init << "," << itr.second->level << "," << itr.second->level_id << "\n";
        }
    }
}