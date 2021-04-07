#include "sym_table.h"

map<string, sym_tab*> func_sym_tab_map;
map<string, string> func_args; 
sym_tab global_tab;
sym_tab* curr;

void sym_tab_init(){
    curr = &global_tab;
    // add reserved keywords in table so they arent used again
}

int make_symbol_table(string func_name){
    if (func_sym_tab_map.find(func_name) != func_sym_tab_map.end()){
        return -1;
    }
    sym_tab* new_tab = new sym_tab;
    func_sym_tab_map[func_name] = new_tab;
}

int insert_entry(string sym_name, unsigned long long size, long long offset, bool init, unsigned long long level, unsigned long long level_id){
    sym_tab_entry* entry = new sym_tab_entry();
    entry->sym_name = sym_name;
    entry->size = size;
    entry->offset = offset;
    entry->init = init;
    entry->level = level;
    entry->level_id = level_id;
    *curr[{entry->sym_name, entry->level, entry->level_id}}] = entry;
    return 0;
}

sym_tab_entry* lookup(string sym_name, unsigned long long level, unsigned long long level_id){
    if (*curr.find({sym_name, level, level_id}).find() != *curr.end()){
        return *curr[{sym_name, level, level_id}];
    }
    return NULL;
}

void dump_tables(){
    string glob = "global_table.csv";
    ofstream out_file(glob);
    for (auto itr : global_tab){
        out_file << itr->second->sym_name << "," << itr->second->size << "," << itr->second->offset << "," << itr->second->init << "," << itr->second->level << "," << itr->second->level_id << "\n";
    }
    for (auto table : func_sym_tab_map){
        string filename = table->first + "_table.csv";
        ofstream out_file(filename);
        for (auto itr : *(table->second)){
            out_file << itr->second->sym_name << "," << itr->second->size << "," << itr->second->offset << "," << itr->second->init << "," << itr->second->level << "," << itr->second->level_id << "\n";
        }
    }
}