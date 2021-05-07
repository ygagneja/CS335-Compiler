#include<bits/stdc++.h>

#define MAIN_AR_SIZE 4
#define FUNC_AR_SIZE 128

using namespace std;

void asmb_header(string s);
void asmb_label(string s);
void asmb_line(string s);
void dump_asm_code();
void initialise_regs();
string get_reg(qid sym);
void code_gen();