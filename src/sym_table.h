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

typedef map<tuple<string, unsigned long long, unsigned long long>, sym_tab_entry*> sym_tab;

typedef struct {
    string sym_name;
    string type;
    unsigned long long size;
    long long offset;
}struct_sym_entry;

typedef map<string, struct_sym_entry*> struct_sym_tab;

typedef struct {
    string type_name;
    unsigned long long size;
    unsigned long long level;
    unsigned long long level_id;
    struct_sym_tab* symbols;
}type_tab_entry;

typedef map<tuple<string, unsigned long long, unsigned long long>, type_tab_entry*> type_tab;

void tab_init();
void set_current_tab(string func_name);
int make_symbol_table(string func_name);
void restore_offset(string type, unsigned long long level, unsigned long long* level_id);
void insert_entry(string sym_name, string type, unsigned long long size, long long offset, bool init, unsigned long long level, unsigned long long level_id);
sym_tab_entry* lookup_decl(string sym_name, unsigned long long level, unsigned long long level_id);
sym_tab_entry* lookup_use(string sym_name, unsigned long long level, unsigned long long* level_id);
void set_struct_scope();
bool insert_struct_symbol(string sym_name, string type, unsigned long long size);
void insert_type_entry(string type_name, unsigned long long size, unsigned long long level, unsigned long long level_id);
type_tab_entry* lookup_type_decl(string type_name, unsigned long long level, unsigned long long level_id);
type_tab_entry* lookup_type_use(string type_name, unsigned long long level, unsigned long long* level_id);
string struct_sym_type(string type, string sym_name, unsigned long long level, unsigned long long* level_id);
unsigned long long get_size(string type, unsigned long long level, unsigned long long* level_id);
bool is_valid_type(string type, unsigned long long level, unsigned long long* level_id);
bool args_to_scope(string func_name, string func_args, string func_symbols, unsigned long long level, unsigned long long* level_id);
bool is_consistent(string func_name, string args);
void insert_func_args(string func_name, string arg_types);
string get_func_args(string func_name);
void update_func_type(string func_name, unsigned long long level, unsigned long long level_id, string type);
string get_func_ret_type(string func_name);
void dump_tables();
void dump_type_tables();