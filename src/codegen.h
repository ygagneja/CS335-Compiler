#include<bits/stdc++.h>

#define MAIN_AR_SIZE 4
#define FUNC_AR_SIZE 8

using namespace std;

void asmb_header(string s);
void asmb_label(string s);
void asmb_line(string s);
void dump_asm_code(bool link_lib_funcs);
void initialise_regs();
string get_reg(void* tmp, bool from_mem = true);
void spill_regs();
void spill_global_regs();
void code_gen(bool link_lib_funcs);