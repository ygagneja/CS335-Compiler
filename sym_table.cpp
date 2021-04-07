#include "sym_table.h"

map<string, sym_tab*> func_sym_tab_map;
map<string, string> func_args; 
sym_tab global_tab;

void sym_tab_init(){
    // add reserved keywords in table so they arent used again
}

int make_symbol_table(string func_name){
    if (func_sym_tab_map.find(func_name) != func_sym_tab_map.end()){
        return -1;
    }
    sym_tab* new_tab = new sym_tab;
    func_sym_tab_map[func_name] = new_tab;
}

sym_tab_entry* make_entry(string sym_name, unsigned long long size, long long offset, bool init, unsigned long long level, unsigned long long level_id){
    sym_tab_entry* entry = new sym_tab_entry();
    entry->sym_name = sym_name;
    entry->size = size;
    entry->offset = offset;
    entry->init = init;
    entry->level = level;
    entry->level_id = level_id;
    return entry;
}

int insert_entry(sym_tab* table, sym_tab_entry* sym_tab_entry){
    if (table.find({sym_tab_entry->sym_name, sym_tab_entry->level, sym_tab_entry->level_id}) != table.end()){
        return -1;
    }
    table[{sym_tab_entry->sym_name, sym_tab_entry->level, sym_tab_entry->level_id}}] = sym_tab_entry;
    return 0;
}

sym_tab_entry* lookup(sym_tab* table, string sym_name, unsigned long long level, unsigned long long level_id){
    if (table.find({sym_name, level, level_id}).find() != table.end()){
        return table[{sym_name, level, level_id}];
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