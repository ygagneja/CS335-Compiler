#include<bits/stdc++.h>
#include<fstream>

using namespace std;

typedef struct {
    string sym_name;
    string type;
    unsigned long long size;
    long long offset;
    bool init;
    unsigned long long level;
    unsigned long long level_id;
}sym_tab_entry;

typedef unordered_map<tuple<string, unsigned long long, unsigned long long>, sym_tab_entry*> sym_tab;

typedef struct {
    string type_name;
    string type;
    unsigned long long size;
    unsigned long long level;
    unsigned long long level_id;
}type_tab_entry;

typedef unordered_map<tuple<string, unsigned long long, unsigned long long>, type_tab_entry*> type_tab;

void tab_init();
void set_current_sym_tab(string func_name);
int make_symbol_table(string func_name);
void insert_entry(string sym_name, string type, unsigned long long size, long long offset, bool init, unsigned long long level, unsigned long long level_id);
sym_tab_entry* lookup(string sym_name, unsigned long long level, unsigned long long level_id);
void insert_type_entry(string type_name, string type, unsigned long long size, unsigned long long level, unsigned long long level_id);
type_tab_entry* lookup_type(string type_name);
unsigned long long get_size(string type);
void insert_func_args(string func_name, string arg_types);
string get_func_args(string func_name);
void dump_tables();