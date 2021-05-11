#include "codegen.h"
#include "3ac.h"
#include "type_check.h"

extern sym_tab global_sym_tab;
extern map<string, sym_tab*> func_sym_tab_map;
extern map<string, vector<sym_tab_entry*>> func_syms_map;
extern map<string, long long> max_offsets;

map <string, qid> reg_to_sym;
map <qid, string> sym_to_place;
queue <string> regs_in_use;
queue <string> fregs_in_use;

string reg1, reg2, reg3;
string curr_func;
int fp_cond = 0;
stack<qid> parameters;
map <qid, long long> global_offsets;
long long global_offset;

vector <string> assembly_code;
vector <string> global_scope_assembly;
bool glob_scope;
int global_gen;

void asmb_header(string s){
  if (glob_scope && global_gen){
    global_scope_assembly.push_back(s);
  }
  else if (!glob_scope && !global_gen) {
    assembly_code.push_back(s);
  }
}

void asmb_label(string s){
  if (glob_scope && global_gen){
    global_scope_assembly.push_back("\t" + s);
  }
  else if (!glob_scope && !global_gen) {
    assembly_code.push_back("\t" + s);
  }
}

void asmb_line(string s){
  if (glob_scope && global_gen){
    global_scope_assembly.push_back("\t\t" + s);
  }
  else if(!glob_scope && !global_gen) {
    assembly_code.push_back("\t\t" + s);
  }
}

void dump_asm_code(bool link_lib_funcs){
  ofstream out_file("out.asm");
  for(string it : assembly_code){
    out_file << it << endl;
    if (it == ".text\n" && link_lib_funcs){
      string line;
      ifstream libfile("lib_funcs.asm");
      while (getline(libfile, line)) {
        out_file << line << endl;
      }
      libfile.close();
    }
    if (it == "\tmain : "){
      out_file << "\t\t# global scope assembly code starts" << endl;
      for(string it2 : global_scope_assembly){
        out_file << it2 << endl;
      }
      out_file << "\t\t# global scope assembly code ends\n" << endl;
    }
  }
  out_file.close();
}

void initialise_regs(){
  for (auto itr : global_sym_tab){
    sym_to_place[(qid)itr.second] = "";
  }
  for (auto table : func_sym_tab_map){
    for (auto itr : *(table.second)){
      sym_to_place[(qid)itr.second] = "";
    }
  }
  // do not initialize the commented regs since they are reserved for special ops
  // gp, fp, ra, sp -> reserved regs
  // v0 and v1 -> return values
  // a0, a1, a2, a3 -> args
  // f0 and f2 -> return values
  // f12, f14, f16, f18 -> args

  reg_to_sym["$t0"] = NULL;
  reg_to_sym["$t1"] = NULL;
  reg_to_sym["$t2"] = NULL;
  reg_to_sym["$t3"] = NULL;
  reg_to_sym["$t4"] = NULL;
  reg_to_sym["$t5"] = NULL;
  reg_to_sym["$t6"] = NULL;
  reg_to_sym["$t7"] = NULL;
  reg_to_sym["$t8"] = NULL;
  reg_to_sym["$t9"] = NULL;
  reg_to_sym["$s0"] = NULL;
  reg_to_sym["$s1"] = NULL;
  reg_to_sym["$s2"] = NULL;
  reg_to_sym["$s3"] = NULL;
  reg_to_sym["$s4"] = NULL;
  reg_to_sym["$s5"] = NULL;
  reg_to_sym["$s6"] = NULL;
  reg_to_sym["$s7"] = NULL;
  reg_to_sym["$f4"] = NULL;
  reg_to_sym["$f6"] = NULL;
  reg_to_sym["$f8"] = NULL;
  reg_to_sym["$f10"] = NULL;
  reg_to_sym["$f20"] = NULL;
  reg_to_sym["$f22"] = NULL;
  reg_to_sym["$f24"] = NULL;
  reg_to_sym["$f26"] = NULL;
  reg_to_sym["$f28"] = NULL;
  reg_to_sym["$f30"] = NULL;
}

string get_reg(void* tmp, bool from_mem){
  qid sym = (qid)tmp;
  // cout << t -> sym_name << " " << t -> offset << endl;
  if (sym_to_place[sym] != ""){
    return sym_to_place[sym];
  }
  long long offset = curr_func == "main" ? MAIN_AR_SIZE : FUNC_AR_SIZE;
  if (is_type_float(sym->type)){
    for (auto itr : reg_to_sym){
      if (itr.first.substr(0, 2) == "$f" && itr.second == NULL){
        // free reg available
        fregs_in_use.push(itr.first);
        reg_to_sym[itr.first] = sym;
        sym_to_place[sym] = itr.first;
        // load val from memory to reg
        offset += sym->offset;
        if (from_mem){
          if (sym->level){
            asmb_line("lwc1 " + itr.first + ", " + to_string(-offset) + "($fp)");
          }
          else {
            asmb_line("lwc1 " + itr.first + ", " + to_string(global_offsets[sym]) + "($gp)");
          }
        } 

        return itr.first;
      }
    }
    // no free reg available
    string reg = fregs_in_use.front();
    fregs_in_use.pop();
    // store val from reg to memory
    offset += reg_to_sym[reg]->offset;
    if (reg_to_sym[reg]->level){
      asmb_line("swc1 " + reg + ", " + to_string(-offset) + "($fp)");
    }
    else {
      asmb_line("swc1 " + reg + ", " + to_string(global_offsets[reg_to_sym[reg]]) + "($gp)");
    }
    sym_to_place[reg_to_sym[reg]] = "";
    // load val from memory to reg
    offset -= reg_to_sym[reg]->offset;
    offset += sym->offset;
    if (from_mem){
      if (sym->level){
        asmb_line("lwc1 " + reg + ", " + to_string(-offset) + "($fp)");
      }
      else {
        asmb_line("lwc1 " + reg + ", " + to_string(global_offsets[sym]) + "($gp)");
      }
    }
    sym_to_place[sym] = reg;
    fregs_in_use.push(reg);
    reg_to_sym[reg] = sym;
    return reg;
  }
  else {
    for (auto itr : reg_to_sym){
      if (itr.first.substr(0, 2) != "$f" && itr.second == NULL){
        // free reg available
        regs_in_use.push(itr.first);
        reg_to_sym[itr.first] = sym;
        sym_to_place[sym] = itr.first;
        // load val from memory to reg
        offset += sym->offset;
        if (from_mem){
          if(is_type_char(sym->type) || is_type_bool(sym->type)){
            asmb_line("move " + itr.first + ", $zero");
            if (sym->level){
              asmb_line("lb " + itr.first + ", " + to_string(-offset) + "($fp)");
            }
            else {
              asmb_line("lb " + itr.first + ", " + to_string(global_offsets[sym]) + "($gp)");
            }
          } 
          else {
            if (sym->level){
              asmb_line("lw " + itr.first + ", " + to_string(-offset) + "($fp)");
            }
            else {
              asmb_line("lw " + itr.first + ", " + to_string(global_offsets[sym]) + "($gp)");
            }
          } 
        }
        return itr.first;
      }
    }
    // no free reg available
    string reg = regs_in_use.front();
    regs_in_use.pop();
    // store val from reg to memory
    offset += reg_to_sym[reg]->offset;
    if(is_type_char(reg_to_sym[reg]->type) || is_type_bool(reg_to_sym[reg]->type)){
      if (reg_to_sym[reg]->level){
        asmb_line("sb " + reg + ", " + to_string(-offset) + "($fp)");
      }
      else {
        asmb_line("sb " + reg + ", " + to_string(global_offsets[reg_to_sym[reg]]) + "($gp)");
      }
    }
    else {
      if (reg_to_sym[reg]->level){
        asmb_line("sw " + reg + ", " + to_string(-offset) + "($fp)");
      }
      else {
        asmb_line("sw " + reg + ", " + to_string(global_offsets[reg_to_sym[reg]]) + "($gp)");
      }
    } 
    sym_to_place[reg_to_sym[reg]] = "";
    // load val from memory to reg
    offset -= reg_to_sym[reg]->offset;
    offset += sym->offset;
    if (from_mem){
      if(is_type_char(sym->type) || is_type_bool(sym->type)){
        asmb_line("move " + reg + ", $zero");
        if (sym->level){
          asmb_line("lb " + reg + ", " + to_string(-offset) + "($fp)");
        }
        else {
          asmb_line("lb " + reg + ", " + to_string(global_offsets[sym]) + "($gp)");
        }
      } 
      else {
        if (sym->level){
          asmb_line("lw " + reg + ", " + to_string(-offset) + "($fp)");
        }
        else {
          asmb_line("lw " + reg + ", " + to_string(global_offsets[sym]) + "($gp)");
        }
      } 
    }
    sym_to_place[sym] = reg;
    regs_in_use.push(reg);
    reg_to_sym[reg] = sym;
    return reg;
  }
}

void spill_regs(){
  long long offset = curr_func == "main" ? MAIN_AR_SIZE : FUNC_AR_SIZE;
  for(auto itr : reg_to_sym){
    if(itr.second){
      offset += itr.second->offset;
      if (is_type_char(itr.second->type) || is_type_bool(itr.second->type)){
        if (itr.second->level){
          asmb_line("sb " + itr.first + ", " + to_string(-offset) + "($fp)");
        }
        else {
          asmb_line("sb " + itr.first + ", " + to_string(global_offsets[itr.second]) + "($gp)");
        }
      }
      else if (is_type_float(itr.second->type)){
        if (itr.second->level){
          asmb_line("swc1 " + itr.first + ", " + to_string(-offset) + "($fp)");
        }
        else {
          asmb_line("swc1 " + itr.first + ", " + to_string(global_offsets[itr.second]) + "($gp)");
        }
      }
      else {
        if (itr.second->level){
          asmb_line("sw " + itr.first + ", " + to_string(-offset) + "($fp)");
        }
        else {
          asmb_line("sw " + itr.first + ", " + to_string(global_offsets[itr.second]) + "($gp)");
        }
      }
      offset -= itr.second->offset;
      sym_to_place[itr.second] = "";
      reg_to_sym[itr.first] = NULL;
    }
  }
}

void spill_global_regs(){
  for(auto itr : reg_to_sym){
    if(itr.second && itr.second->level == 0){
      if (is_type_char(itr.second->type) || is_type_bool(itr.second->type)){
        asmb_line("sb " + itr.first + ", " + to_string(global_offsets[itr.second]) + "($gp)");
      }
      else if (is_type_float(itr.second->type)){
        asmb_line("swc1 " + itr.first + ", " + to_string(global_offsets[itr.second]) + "($gp)");
      }
      else {
        asmb_line("sw " + itr.first + ", " + to_string(global_offsets[itr.second]) + "($gp)");
      }
      sym_to_place[itr.second] = "";
      reg_to_sym[itr.first] = NULL;
    }
  }
}

void code_gen(bool link_lib_funcs){
  initialise_regs();

  glob_scope = false;
  asmb_header(".data\n");
  for (auto itr : global_sym_tab){
    if (itr.second->type.substr(0, 5) != "func "){
      if (global_offset % 4 && itr.second->size >= 4){
        asmb_label(".align\t2");
        global_offset += (4 - global_offset % 4);
        global_offsets[itr.second] = global_offset;
        global_offset += itr.second->size;
      }
      else {
        global_offsets[itr.second] = global_offset;
        global_offset += itr.second->size;
      }
      string append = itr.second->sym_name + " :\t .space " + to_string(itr.second->size);
      asmb_label(append);
    }
  }

  asmb_header(".text\n");
  // scan_int assembly
  asmb_label("scani : ");
  asmb_line("li $v0, 5");
  asmb_line("syscall");
  asmb_line("jr $ra");
  // scan_float assembly
  asmb_label("scanf : ");
  asmb_line("li $v0, 6");
  asmb_line("syscall");
  asmb_line("jr $ra");
  // scan_string assembly
  asmb_label("scans : ");
  asmb_line("li $v0, 8");
  asmb_line("syscall");
  asmb_line("jr $ra");
  // print_int assembly
  asmb_label("printi : ");
  asmb_line("li $v0, 1");
  asmb_line("syscall");
  asmb_line("li $a0, 0xA");
  asmb_line("li $v0, 11");
  asmb_line("syscall");
  asmb_line("jr $ra");
  // print_float assembly
  asmb_label("printf : ");
  asmb_line("li $v0, 2");
  asmb_line("syscall");
  asmb_line("li $a0, 0xA");
  asmb_line("li $v0, 11");
  asmb_line("syscall");
  asmb_line("jr $ra");
  // print_string assembly
  asmb_label("prints : ");
  asmb_line("li $v0, 4");
  asmb_line("syscall");
  asmb_line("li $a0, 0xA");
  asmb_line("li $v0, 11");
  asmb_line("syscall");
  asmb_line("jr $ra");
  // malloc assembly
  asmb_label("malloc : ");
  asmb_line("li $v0, 9");
  asmb_line("syscall");
  asmb_line("jr $ra");

  glob_scope = true;

  // Store goto label locations to create label in assembly
  unordered_set<int> label_lines;
  for (int i=0; i<code_arr.size(); i++){
    if (code_arr[i].op == "GOTO" || code_arr[i].op == "GOTO IF"){
      int tmp = code_arr[i].goto_label;
      while (code_arr[tmp].op == "GOTO") tmp = code_arr[tmp].goto_label;
      label_lines.insert(tmp);
      code_arr[i].goto_label = tmp;
    }
  }

  for (global_gen=1; global_gen>=0; global_gen--){

    for (int i=0; i<code_arr.size(); i++){
      
      if (!global_gen && i==0){
        glob_scope = false;
        while (i<code_arr.size() && code_arr[i].op.substr(0, 16) != "*function start*") i++;
      }
      
      if(label_lines.count(i)){
        spill_regs();
        asmb_label("_jump_label_" + to_string(i) + " :");
      }

      if(code_arr[i].op.substr(0, 14) == "*function end*"){
        if (curr_func == "main"){
          asmb_label("main_end : ");
          asmb_line("li $v0, 10 \t # Exit");
          asmb_line("syscall");
        }
        else {
          asmb_label(curr_func + "_end : ");
          spill_global_regs();
          asmb_line("lw $sp, -8($fp) \t # load caller func size");
          asmb_line("lw $ra, -4($fp) \t # restore return address");
          asmb_line("addu $fp, $fp, $sp \t # restore frame pointer");
          // asmb_line("lw $fp, " + to_string(FUNC_AR_SIZE-4) + "($sp) \t # Restore frame pointer");
          // asmb_line("lw $ra, " + to_string(FUNC_AR_SIZE-8) + "($sp) \t # Restore return address");
          // asmb_line("addu $sp, $sp, " + to_string(FUNC_AR_SIZE) + "\t # Restore stack pointer");
          asmb_line("jr $ra \t # Return");
        }
        initialise_regs();
        if (!global_gen){
          while (i<code_arr.size() && code_arr[i].op.substr(0, 16) != "*function start*") i++;
          i--;
          continue;
        }
        glob_scope = true;
      }
      else if(code_arr[i].op.substr(0, 16) == "*function start*"){
        // cout << "Assembly for function start\n";
        if (global_gen){
          while (i<code_arr.size() && code_arr[i].op.substr(0, 14) != "*function end*") i++;
          continue;
        }
        glob_scope = false;
        // Extract the name of the func from op
        curr_func = code_arr[i].op.substr(18);
        curr_func.pop_back();

        if(curr_func == "main"){
          asmb_label("main : ");
          asmb_line("move $fp, $sp");
        }
        else{
          asmb_label(curr_func + " : ");
          asmb_line("subu $fp, $fp, $sp");
          asmb_line("sw $sp, -8($fp) \t # preserve func size");
          asmb_line("sw $ra, -4($fp) \t # preserve return address");

          // move args to temp regs
          int non_f_args = 0, f_args = 0;
          for (int i=0; i<func_syms_map[curr_func].size(); i++){
            qid sym = (qid)func_syms_map[curr_func][i];
            string reg = get_reg(sym, false);
            if (is_type_float(sym->type)){
              asmb_line("mov.s " + reg + ", $f" + to_string(f_args*2 + 12) + "\t # get value of " + sym->sym_name);
              f_args++;
            }
            else {
              asmb_line("move " + reg + ", $a" + to_string(non_f_args) + "\t # get value of " + sym->sym_name);
              non_f_args++;
            }
          }
        }
      }
      else if(code_arr[i].op == "params"){ // <- params a
          // cout << "Printing params assembly \n";
          if (code_arr[i].arg2) parameters.push(code_arr[i].arg2);
      }
      else if(code_arr[i].op == "call"){ // res <- call curr_func
        string callee = code_arr[i].arg2->sym_name;
        int non_f_args = 0, f_args = 0;
        for (int i=0; i<func_syms_map[callee].size(); i++){
          if (is_type_float(func_syms_map[callee][i]->type)) f_args++;
          else non_f_args++;
        }

        for (int i=0; i<func_syms_map[callee].size(); i++){
          qid sym = parameters.top();
          parameters.pop();
          string reg = get_reg(sym);
          if (is_type_float(sym->type)){
            f_args--;
            asmb_line("mov.s $f" + to_string(f_args*2 + 12) + ", " + reg + "\t # store value of " + sym->sym_name);
          }
          else {
            non_f_args--;
            asmb_line("move $a" + to_string(non_f_args) + ", " + reg + "\t # store value of " + sym->sym_name);
          }
        }
        // spill regs
        spill_regs();
        // jump to callee
        asmb_line("li $sp, " + to_string(max_offsets[curr_func]));
        asmb_line("jal " + callee);
        if(code_arr[i].res){ // Call to non void function
            string res_reg = get_reg(code_arr[i].res, false);
            if (is_type_float(code_arr[i].res->type)) asmb_line("mov.s " + res_reg + ", $f0 \t # get return value in " + code_arr[i].res->sym_name);
            else asmb_line("move " + res_reg + ", $v0 \t # Get return value in " + code_arr[i].res->sym_name);
        }
      }
      else if(code_arr[i].op == "RETURN"){
        if(code_arr[i].arg2){ // for non-void return types
          string reg = get_reg(code_arr[i].arg2);
          if (is_type_float(code_arr[i].arg2->type)) asmb_line("mov.s $f0, " + reg);
          else asmb_line("move $v0, " + reg);
        }
        asmb_line("j " + curr_func + "_end"); // Branch to function end directive
      }
      else if(code_arr[i].op == "="){  // res <- = arg2, arg2 could be a constant(num) or a temporary
        // cout << "Printing = assembly\n";
        string res_reg = get_reg(code_arr[i].res, false);
        if(code_arr[i].arg2){
            string arg_reg = get_reg(code_arr[i].arg2); // arg2 is a temporary
            if(is_type_float(code_arr[i].arg2->type)) asmb_line("mov.s " + res_reg + ", " + arg_reg + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg2->sym_name);
            else asmb_line("move " + res_reg + ", " + arg_reg + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg2->sym_name);
        }
        else {
          if(is_type_float(code_arr[i].res->type)) asmb_line("li.s " + res_reg + ", " + code_arr[i].constant + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].constant);
          else asmb_line("li " + res_reg + ", " + code_arr[i].constant + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].constant);
        }
      }
      else if(code_arr[i].op == "* ="){
        string arg_reg = get_reg(code_arr[i].arg2);
        string addr_reg = get_reg(code_arr[i].res);
        if(is_type_float(code_arr[i].arg2->type)) asmb_line("swc1 " + arg_reg + ", (" + addr_reg + ")" + "\t # "+code_arr[i].res->sym_name + " * = "+code_arr[i].arg2->sym_name);
        else if(is_type_bool(code_arr[i].arg2->type) || is_type_char(code_arr[i].arg2->type)) asmb_line("sb " + arg_reg + ", (" + addr_reg + ")" + "\t # "+code_arr[i].res->sym_name + " * = "+code_arr[i].arg2->sym_name);
        else asmb_line("sw " + arg_reg + ", (" + addr_reg + ")" + "\t # "+code_arr[i].res->sym_name + " * = "+code_arr[i].arg2->sym_name);
      }
      else if(code_arr[i].op == "=string"){
        long long offset = curr_func == "main" ? MAIN_AR_SIZE : FUNC_AR_SIZE;
        offset += code_arr[i].res->offset;
        string s = code_arr[i].constant;
        for (int j=0; j<s.size(); j++){
          asmb_line("li $a0, " + to_string((int)s[j]) + "\t # load character to register");
          if (code_arr[i].res->level) asmb_line("sb $a0, " + to_string(-offset + j) + "($fp)" + "\t # store character to memory");
          else asmb_line("sb $a0, " + to_string(global_offsets[code_arr[i].res] + j) + "($gp)" + "\t # store character to memory");
        }
        if (code_arr[i].res->level) asmb_line("sb $0, " + to_string(-offset + (int)s.size()) + "($fp)" + "\t # store character to memory");
        else asmb_line("sb $a0, " + to_string(global_offsets[code_arr[i].res] + s.size()) + "($fp)" + "\t # store character to memory");
      }
      else if(code_arr[i].op == "&"){ // res <- &arg2
        // cout << "Printing unary& assembly\n";
        if (code_arr[i].arg1){
          reg1 = get_reg(code_arr[i].arg1);
          reg2 = get_reg(code_arr[i].arg2);
          reg3 = get_reg(code_arr[i].res, false);
          asmb_line("and " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " & " + code_arr[i].arg2->sym_name);
        }
        else {
          string res_reg = get_reg(code_arr[i].res, false);
          long long offset = curr_func == "main" ? MAIN_AR_SIZE : FUNC_AR_SIZE;
          offset += code_arr[i].arg2->offset;
          if (code_arr[i].arg2->level) asmb_line("subu " + res_reg + ", $fp, " + to_string(offset) + "\t # "+code_arr[i].res->sym_name + " = &"+code_arr[i].arg2->sym_name);
          else asmb_line("addu " + res_reg + ", $gp, " + to_string(global_offsets[code_arr[i].arg2]) + "\t # "+code_arr[i].res->sym_name + " = &"+code_arr[i].arg2->sym_name);
          if (!is_type_ptr(code_arr[i].arg2->type) && !is_type_struct(code_arr[i].arg2->type)){
            string arg_reg = get_reg(code_arr[i].arg2);
            if(is_type_float(code_arr[i].arg2->type)) asmb_line("swc1 " + arg_reg + ", (" + res_reg + ")" + "\t # storing in memory to avoid wild loads");
            else if(is_type_bool(code_arr[i].arg2->type) || is_type_char(code_arr[i].arg2->type)) asmb_line("sb " + arg_reg + ", (" + res_reg + ")" + "\t # storing in memory to avoid wild loads");
            else asmb_line("sw " + arg_reg + ", (" + res_reg + ")" + "\t # storing in memory to avoid wild loads");
          }
        }
      }
      else if(code_arr[i].op == "*"){ // res <- *arg2
        // cout << "Printing unary* assembly\n";
        string res_reg = get_reg(code_arr[i].res, false);
        string arg_reg = get_reg(code_arr[i].arg2);
        if (is_type_float(code_arr[i].res->type)) asmb_line("lwc1 " + res_reg + ", (" + arg_reg + ")" + "\t # "+code_arr[i].res->sym_name + " = *"+code_arr[i].arg2->sym_name);
        else if (is_type_bool(code_arr[i].res->type) || is_type_char(code_arr[i].res->type)) asmb_line("lb " + res_reg + ", (" + arg_reg + ")" + "\t # "+code_arr[i].res->sym_name + " = *"+code_arr[i].arg2->sym_name);
        else asmb_line("lw " + res_reg + ", (" + arg_reg + ")" + "\t # "+code_arr[i].res->sym_name + " = *"+code_arr[i].arg2->sym_name);
      }
      else if(code_arr[i].op == "-"){ // res <- -arg2
        // cout << "Printing unary- assembly\n";
        string res_reg = get_reg(code_arr[i].res, false);
        string arg_reg = get_reg(code_arr[i].arg2);
        if(is_type_float(code_arr[i].arg2 -> type)) asmb_line("neg.s " + res_reg + ", " + arg_reg + "\t # "+code_arr[i].res->sym_name + " = -"+code_arr[i].arg2->sym_name);
        else asmb_line("neg " + res_reg + ", " + arg_reg + "\t # "+code_arr[i].res->sym_name + " = -"+code_arr[i].arg2->sym_name);
      }
      else if(code_arr[i].op == "~"){ // res <- ~arg2
        // cout << "Printing unary ~ assembly\n";
        string res_reg = get_reg(code_arr[i].res, false);
        string arg_reg = get_reg(code_arr[i].arg2);
        asmb_line("not " + res_reg + ", " + arg_reg + "\t # "+code_arr[i].res->sym_name + " = ~"+code_arr[i].arg2->sym_name);
        if (is_type_char(code_arr[i].res->type)){
          asmb_line("and " + res_reg + ", " + res_reg + ", 0xFF");
        }
        else if (is_type_bool(code_arr[i].res->type)){
          asmb_line("and " + res_reg + ", " + res_reg + ", 0x1");
        }
      }
      else if(code_arr[i].op == "!"){ // res <- !arg2
        // cout << "Printing unary ! assembly\n";
        string res_reg = get_reg(code_arr[i].res, false);
        string arg_reg = get_reg(code_arr[i].arg2);
        asmb_line("seq " + res_reg + ", " + arg_reg + ", " + "$zero" + "\t # "+code_arr[i].res->sym_name + " = !"+code_arr[i].arg2->sym_name);
      }
      // addition
      else if(code_arr[i].op == "+int" || code_arr[i].op == "+float" || code_arr[i].op == "+ptr"){
        // cout << "Printing addition assembly\n";
        if (code_arr[i].arg2){
          string res_reg = get_reg(code_arr[i].res, false);
          string arg1_reg = get_reg(code_arr[i].arg1);
          string arg2_reg = get_reg(code_arr[i].arg2);
          if(code_arr[i].op == "+int") asmb_line("add " + res_reg + ", " + arg1_reg + ", " + arg2_reg + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " +int "+code_arr[i].arg2->sym_name);
          else if(code_arr[i].op == "+ptr") asmb_line("addu " + res_reg + ", " + arg1_reg + ", " + arg2_reg + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " +ptr "+code_arr[i].arg2->sym_name);
          else asmb_line("add.s " + res_reg + ", " + arg1_reg + ", " + arg2_reg + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " +float "+code_arr[i].arg2->sym_name);
        }
        else {
          string res_reg = get_reg(code_arr[i].res, false);
          string arg1_reg = get_reg(code_arr[i].arg1);
          if(code_arr[i].op == "+int") asmb_line("add " + res_reg + ", " + arg1_reg + ", " + code_arr[i].constant + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " +int "+code_arr[i].constant);
          else if(code_arr[i].op == "+ptr") asmb_line("addu " + res_reg + ", " + arg1_reg + ", " + code_arr[i].constant + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " +ptr "+code_arr[i].constant);
          else asmb_line("add.s " + res_reg + ", " + arg1_reg + ", " + code_arr[i].constant + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " +float "+code_arr[i].constant);
        }
      }
      // subtraction
      else if(code_arr[i].op == "-int" || code_arr[i].op == "-float" || code_arr[i].op == "-ptr"){
        // cout << "Printing subtraction assembly\n";
        string res_reg = get_reg(code_arr[i].res, false);
        string arg1_reg = get_reg(code_arr[i].arg1);
        string arg2_reg = get_reg(code_arr[i].arg2);
        if(code_arr[i].op == "-int") asmb_line("sub " + res_reg + ", " + arg1_reg + ", " + arg2_reg + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " -int " + code_arr[i].arg2->sym_name);
        else if(code_arr[i].op == "-ptr") asmb_line("subu " + res_reg + ", " + arg1_reg + ", " + arg2_reg + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " -ptr " + code_arr[i].arg2->sym_name);
        else asmb_line("sub.s " + res_reg + ", " + arg1_reg + ", " + arg2_reg + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " -float " + code_arr[i].arg2->sym_name);
      }
      // Integer multiplication
      else if(code_arr[i].op == "*int" || code_arr[i].op == "*float"){
        // cout << "Printing multiplication assembly\n";
        string res_reg = get_reg(code_arr[i].res, false);
        string arg1_reg = get_reg(code_arr[i].arg1);
        if (code_arr[i].arg2){
            string arg2_reg = get_reg(code_arr[i].arg2);
            if(code_arr[i].op == "*int") asmb_line("mul " + res_reg + ", " + arg1_reg + ", " + arg2_reg + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " *int "+code_arr[i].arg2->sym_name);
            else if(code_arr[i].op == "*float") asmb_line("mul.s " + res_reg + ", " + arg1_reg + ", " + arg2_reg + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " *float "+code_arr[i].arg2->sym_name);
        }
        else{
            if(code_arr[i].op == "*int"){
              asmb_line("mul " + res_reg + ", " + arg1_reg + ", " + code_arr[i].constant + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " *int "+code_arr[i].constant);
            }
            else if(code_arr[i].op == "*float"){
                asmb_line("li.s " + res_reg + ", " + code_arr[i].constant);
                asmb_line("mul.s " + res_reg + ", " + arg1_reg + ", " + res_reg + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " *float "+code_arr[i].constant);
            }
        }
      }
      // Integer division
      else if(code_arr[i].op == "/int" || code_arr[i].op == "/float"){
        //  cout << "Printing division assembly\n";
        string res_reg = get_reg(code_arr[i].res, false);
        string arg1_reg = get_reg(code_arr[i].arg1);
        string arg2_reg = get_reg(code_arr[i].arg2);
        if(code_arr[i].op == "/int") asmb_line("div " + res_reg + ", " + arg1_reg + ", " + arg2_reg + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " /int "+code_arr[i].arg2->sym_name);
        else if(code_arr[i].op == "/float") asmb_line("div.s " + res_reg + ", " + arg1_reg + ", " + arg2_reg + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " /float "+code_arr[i].arg2->sym_name);
      }
      // Integer modulo
      else if(code_arr[i].op == "%int"){
        // cout << "Printing modulo assembly\n";
        string res_reg = get_reg(code_arr[i].res, false);
        string arg1_reg = get_reg(code_arr[i].arg1);
        string arg2_reg = get_reg(code_arr[i].arg2);
        asmb_line("div " + arg1_reg +", " + arg2_reg + "\t # "+code_arr[i].arg1->sym_name + "/" + code_arr[i].arg2->sym_name);
        asmb_line("mfhi " + res_reg + "\t # store modulo in "+code_arr[i].res->sym_name);
      }
      else if(code_arr[i].op == "<int" || code_arr[i].op == "<float" || code_arr[i].op == "<ptr"){
        reg1 = get_reg(code_arr[i].arg1);
        reg2 = get_reg(code_arr[i].arg2);
        reg3 = get_reg(code_arr[i].res, false);
        if(code_arr[i].op == "<int") asmb_line("slt " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " <int "+code_arr[i].arg2->sym_name+"? 1 : 0");
        else if(code_arr[i].op == "<ptr") asmb_line("sltu " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " <ptr "+code_arr[i].arg2->sym_name+"? 1 : 0");
        else {
          asmb_line("c.lt.s " + reg1 + ", " + reg2+"\t # "+code_arr[i].arg1->sym_name + " <float " + code_arr[i].arg2->sym_name + " ?");
          asmb_line("bc1t _fp_cond_true_" + to_string(fp_cond));
          asmb_line("li " + reg3 + ", 0");
          asmb_line("b _fp_cond_end_" + to_string(fp_cond));
          asmb_label("_fp_cond_true_" + to_string(fp_cond) + " : ");
          asmb_line("li " + reg3 + ", 1");
          asmb_label("_fp_cond_end_" + to_string(fp_cond) + " : ");
          fp_cond++;
        } 
      }
      // >
      else if(code_arr[i].op == ">int" || code_arr[i].op == ">float" || code_arr[i].op == ">ptr"){
          reg1 = get_reg(code_arr[i].arg1);
          reg2 = get_reg(code_arr[i].arg2);
          reg3 = get_reg(code_arr[i].res, false);
          if(code_arr[i].op == ">int") asmb_line("sgt " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " >int "+code_arr[i].arg2->sym_name+"? 1 : 0");
          else if(code_arr[i].op == ">ptr") asmb_line("sgtu " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " >ptr "+code_arr[i].arg2->sym_name+"? 1 : 0");
          else {
            asmb_line("c.le.s " + reg1 + ", " + reg2+"\t # "+code_arr[i].arg1->sym_name + " >float " + code_arr[i].arg2->sym_name + " ?");
            asmb_line("bc1t _fp_cond_true_" + to_string(fp_cond));
            asmb_line("li " + reg3 + ", 1");
            asmb_line("b _fp_cond_end_" + to_string(fp_cond));
            asmb_label("_fp_cond_true_" + to_string(fp_cond) + " : ");
            asmb_line("li " + reg3 + ", 0");
            asmb_label("_fp_cond_end_" + to_string(fp_cond) + " : ");
            fp_cond++;
          }
      }
      // >=
      else if(code_arr[i].op == ">=int" || code_arr[i].op == ">=float" || code_arr[i].op == ">=ptr"){
        reg1 = get_reg(code_arr[i].arg1);
        reg2 = get_reg(code_arr[i].arg2);
        reg3 = get_reg(code_arr[i].res, false);
        if(code_arr[i].op == ">=int") asmb_line("sge " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " >=int "+code_arr[i].arg2->sym_name+"? 1 : 0");
        else if(code_arr[i].op == ">=ptr") asmb_line("sgeu " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " >=ptr "+code_arr[i].arg2->sym_name+"? 1 : 0");
        else {
          asmb_line("c.lt.s " + reg1 + ", " + reg2+"\t # "+code_arr[i].arg1->sym_name + " >=float " + code_arr[i].arg2->sym_name + " ?");
          asmb_line("bc1t _fp_cond_true_" + to_string(fp_cond));
          asmb_line("li " + reg3 + ", 1");
          asmb_line("b _fp_cond_end_" + to_string(fp_cond));
          asmb_label("_fp_cond_true_" + to_string(fp_cond) + " : ");
          asmb_line("li " + reg3 + ", 0");
          asmb_label("_fp_cond_end_" + to_string(fp_cond) + " : ");
          fp_cond++;
        }
      }
      // <=
      else if(code_arr[i].op == "<=int" || code_arr[i].op == "<=float" || code_arr[i].op == "<=ptr"){
        reg1 = get_reg(code_arr[i].arg1);
        reg2 = get_reg(code_arr[i].arg2);
        reg3 = get_reg(code_arr[i].res, false);
        if(code_arr[i].op == "<=int") asmb_line("sle " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " <=int "+code_arr[i].arg2->sym_name+"? 1 : 0");
        else if(code_arr[i].op == "<=ptr") asmb_line("sleu " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " <=ptr "+code_arr[i].arg2->sym_name+"? 1 : 0");
        else {
          asmb_line("c.le.s " + reg1 + ", " + reg2+"\t # "+code_arr[i].arg1->sym_name + " <=float " + code_arr[i].arg2->sym_name + " ?");
          asmb_line("bc1t _fp_cond_true_" + to_string(fp_cond));
          asmb_line("li " + reg3 + ", 0");
          asmb_line("b _fp_cond_end_" + to_string(fp_cond));
          asmb_label("_fp_cond_true_" + to_string(fp_cond) + " : ");
          asmb_line("li " + reg3 + ", 1");
          asmb_label("_fp_cond_end_" + to_string(fp_cond) + " : ");
          fp_cond++;
        }
      }
      // ==
      else if(code_arr[i].op == "==int" || code_arr[i].op == "==float" || code_arr[i].op == "==ptr"){
        // cout << "Checking Equality\n";
        if(!code_arr[i].arg2){
          // arg2 is never NULL for ==ptr (for now)
          string res_reg = get_reg(code_arr[i].res, false);
          string arg_reg = get_reg(code_arr[i].arg1);
          if(code_arr[i].op == "==int") asmb_line("seq " + reg3 + ", " + reg1 + ", " + code_arr[i].constant + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " ==int "+code_arr[i].constant+"? 1 : 0");
          else if(code_arr[i].op == "==ptr") asmb_line("seq " + reg3 + ", " + reg1 + ", " + code_arr[i].constant + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " ==int "+code_arr[i].constant+"? 1 : 0");
          else {
            asmb_line("c.eq.s " + reg1 + ", " + code_arr[i].constant+"\t # "+code_arr[i].arg1->sym_name + " ==float " + code_arr[i].constant + " ?");
            asmb_line("bc1t _fp_cond_true_" + to_string(fp_cond));
            asmb_line("li " + reg3 + ", 0");
            asmb_line("b _fp_cond_end_" + to_string(fp_cond));
            asmb_label("_fp_cond_true_" + to_string(fp_cond) + " : ");
            asmb_line("li " + reg3 + ", 1");
            asmb_label("_fp_cond_end_" + to_string(fp_cond) + " : ");
            fp_cond++;
          }
        }
        else{
          reg1 = get_reg(code_arr[i].arg1);
          reg2 = get_reg(code_arr[i].arg2);
          reg3 = get_reg(code_arr[i].res, false);
          if(code_arr[i].op == "==int") asmb_line("seq " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " ==int "+code_arr[i].arg2->sym_name+"? 1 : 0");
          else if(code_arr[i].op == "==ptr") asmb_line("seq " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " ==ptr "+code_arr[i].arg2->sym_name+"? 1 : 0");
          else {
            asmb_line("c.eq.s " + reg1 + ", " + reg2+"\t # "+code_arr[i].arg1->sym_name + " ==float " + code_arr[i].arg2->sym_name + " ?");
            asmb_line("bc1t _fp_cond_true_" + to_string(fp_cond));
            asmb_line("li " + reg3 + ", 0");
            asmb_line("b _fp_cond_end_" + to_string(fp_cond));
            asmb_label("_fp_cond_true_" + to_string(fp_cond) + " : ");
            asmb_line("li " + reg3 + ", 1");
            asmb_label("_fp_cond_end_" + to_string(fp_cond) + " : ");
            fp_cond++;
          }
        }
      }
      // !=
      else if(code_arr[i].op == "!=int" || code_arr[i].op == "!=float" || code_arr[i].op == "!=ptr"){
        reg1 = get_reg(code_arr[i].arg1);
        reg2 = get_reg(code_arr[i].arg2);
        reg3 = get_reg(code_arr[i].res, false);
        if(code_arr[i].op == "!=int") asmb_line("sne " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " !=int "+code_arr[i].arg2->sym_name+"? 1 : 0");
        else if(code_arr[i].op == "!=ptr") asmb_line("sne " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " !=ptr "+code_arr[i].arg2->sym_name+"? 1 : 0");
        else {
            asmb_line("c.eq.s " + reg1 + ", " + reg2+"\t # "+code_arr[i].arg1->sym_name + " !=float " + code_arr[i].arg2->sym_name + " ?");
            asmb_line("bc1t _fp_cond_true_" + to_string(fp_cond));
            asmb_line("li " + reg3 + ", 1");
            asmb_line("b _fp_cond_end_" + to_string(fp_cond));
            asmb_label("_fp_cond_true_" + to_string(fp_cond) + " : ");
            asmb_line("li " + reg3 + ", 0");
            asmb_label("_fp_cond_end_" + to_string(fp_cond) + " : ");
            fp_cond++;
        }
      }
      else if(code_arr[i].op == "E++"){ 
        reg1 = get_reg(code_arr[i].res, false);
        reg2 = get_reg(code_arr[i].arg1);
        if(is_type_float(code_arr[i].arg1 -> type)) {
          asmb_line("mov.s " + reg1 + ", " + reg2);
          asmb_line("add.s " + reg2 + ", " + reg2 + ", 1");
        }
        else if (is_type_char(code_arr[i].arg1 -> type)) {
          asmb_line("move " + reg1 + ", " + reg2);
          asmb_line("addu " + reg2 + ", " + reg2 + ", 1");
        }
        else {
          asmb_line("move " + reg1 + ", " + reg2);
          asmb_line("addi " + reg2 + ", " + reg2 + ", 1");
        }
      }
      else if(code_arr[i].op == "++E"){ 
        reg1 = get_reg(code_arr[i].res, false);
        reg2 = get_reg(code_arr[i].arg1);
        if(is_type_float(code_arr[i].arg1 -> type)) {
          asmb_line("add.s " + reg2 + ", " + reg2 + ", 1");
          asmb_line("mov.s " + reg1 + ", " + reg2);
        }
        else if (is_type_char(code_arr[i].arg1 -> type)) {
          asmb_line("addu " + reg2 + ", " + reg2 + ", 1");
          asmb_line("move " + reg1 + ", " + reg2);
        }
        else {
          asmb_line("addi " + reg2 + ", " + reg2 + ", 1");
          asmb_line("move " + reg1 + ", " + reg2);
        }
      }
      else if(code_arr[i].op == "E--"){ 
        reg1 = get_reg(code_arr[i].res, false);
        reg2 = get_reg(code_arr[i].arg1);
        if(is_type_float(code_arr[i].arg1 -> type)) {
          asmb_line("mov.s " + reg1 + ", " + reg2);
          asmb_line("sub.s " + reg2 + ", " + reg2 + ", 1");
        }
        else if (is_type_char(code_arr[i].arg1 -> type)) {
          asmb_line("move " + reg1 + ", " + reg2);
          asmb_line("subu " + reg2 + ", " + reg2 + ", 1");
        }
        else {
          asmb_line("move " + reg1 + ", " + reg2);
          asmb_line("subi " + reg2 + ", " + reg2 + ", 1");
        }
      }
      else if(code_arr[i].op == "--E"){ 
        reg1 = get_reg(code_arr[i].res, false);
        reg2 = get_reg(code_arr[i].arg1);
        if(is_type_float(code_arr[i].arg1 -> type)) {
          asmb_line("sub.s " + reg2 + ", " + reg2 + ", 1");
          asmb_line("mov.s " + reg1 + ", " + reg2);
        }
        else if (is_type_char(code_arr[i].arg1 -> type)) {
          asmb_line("subu " + reg2 + ", " + reg2 + ", 1");
          asmb_line("move " + reg1 + ", " + reg2);
        }
        else {
          asmb_line("subi " + reg2 + ", " + reg2 + ", 1");
          asmb_line("move " + reg1 + ", " + reg2);
        }
      }
      else if(code_arr[i].op == "GOTO"){ // GOTO
        if (i>0 && code_arr[i-1].op == "GOTO IF"){
          asmb_line("j _jump_label_" + to_string(code_arr[i].goto_label));
        }
        else {
          spill_regs();
          asmb_line("j _jump_label_" + to_string(code_arr[i].goto_label));
        }
      }
      else if(code_arr[i].op == "GOTO IF"){ // GOTO IF
        string arg_reg = get_reg(code_arr[i].arg2);
        asmb_line("move $a0, " + arg_reg);
        spill_regs();
        asmb_line("bnez $a0, _jump_label_" + to_string(code_arr[i].goto_label));
      }
      else if(code_arr[i].op == "inttochar"){ // Load only one byte(lower 8 bits)
        string res_reg = get_reg(code_arr[i].res, false);
        string arg_reg = get_reg(code_arr[i].arg2);
        asmb_line("and " + res_reg + ", " + arg_reg + ", 0xFF \t # "+code_arr[i].res->sym_name + " = int_to_char " + code_arr[i].arg2->sym_name);
      }
      else if(code_arr[i].op == "floattochar"){
        string res_reg = get_reg(code_arr[i].res, false);
        string arg_reg = get_reg(code_arr[i].arg2);
        asmb_line("cvt.w.s $f0, " + arg_reg); // Float to int
        asmb_line("mfc1 " + res_reg + ", $f0"); // move from float reg to int reg
        asmb_line("and " + res_reg + ", " + res_reg + ", 0xFF \t # "+code_arr[i].res->sym_name + " = float_to_char " + code_arr[i].arg2->sym_name);
      }
      else if(code_arr[i].op == "chartofloat"){
        string res_reg = get_reg(code_arr[i].res, false);
        string arg_reg = get_reg(code_arr[i].arg2);
        asmb_line("mtc1 " + arg_reg + ", " + arg_reg); // Intreg to floatreg
        asmb_line("cvt.s.w " + res_reg + ", " + res_reg); // Convert int to float
      }
      else if(code_arr[i].op == "chartoint"){
        string res_reg = get_reg(code_arr[i].res, false);
        string arg_reg = get_reg(code_arr[i].arg2);
        asmb_line("move " + res_reg + ", " + arg_reg + "\t # "+code_arr[i].res->sym_name + " = char_to_int " + code_arr[i].arg2->sym_name);
      }
      else if(code_arr[i].op == "inttobool" || code_arr[i].op == "chartobool"){
        string res_reg = get_reg(code_arr[i].res, false);
        string arg_reg = get_reg(code_arr[i].arg2);
        asmb_line("sne " + res_reg + ", " + arg_reg + ", 0" + "\t # "+code_arr[i].res->sym_name + " = int_to_bool " + code_arr[i].arg2->sym_name); // arg != 0 implies res = 1
      }
      else if(code_arr[i].op == "booltoint" || code_arr[i].op == "booltochar"){
        string res_reg = get_reg(code_arr[i].res, false);
        string arg_reg = get_reg(code_arr[i].arg2);
        asmb_line("sne " + res_reg + ", " + arg_reg + ", 0" + "\t # "+code_arr[i].res->sym_name + " = bool_to_(int/char) " + code_arr[i].arg2->sym_name); // arg == 1 implies res = 1
      }
      else if(code_arr[i].op == "booltofloat"){
        string res_reg = get_reg(code_arr[i].res, false);
        string arg_reg = get_reg(code_arr[i].arg2);
        asmb_line("mtc1 " + arg_reg + ", " + res_reg); // move to float reg
        asmb_line("cvt.s.w " + res_reg + ", " + res_reg); // convert int to float
      }
      else if(code_arr[i].op == "inttofloat"){
        string res_reg = get_reg(code_arr[i].res, false);
        string arg_reg = get_reg(code_arr[i].arg2);
        asmb_line("mtc1 " + arg_reg + ", " + res_reg); // move from int reg to floating point reg
        asmb_line("cvt.s.w " + res_reg + ", " + res_reg); // convert int value to floating point value
      }
      else if(code_arr[i].op == "floattoint"){ 
        string res_reg = get_reg(code_arr[i].res, false);
        string arg_reg = get_reg(code_arr[i].arg2);
        asmb_line("cvt.w.s $f0, " + arg_reg); // convert floating point value to integer value
        asmb_line("mfc1 " + res_reg + ", $f0");  // move from floating point reg to int reg
      }
      else if(code_arr[i].op == "floattobool"){
        string res_reg = get_reg(code_arr[i].res, false);
        string arg_reg = get_reg(code_arr[i].arg2);
        asmb_line("cvt.w.s $f0, " + arg_reg); // convert floating point value to integer value
        asmb_line("mfc1 " + res_reg + ", $f0");  // move from floating point reg to int reg
        asmb_line("sne " + res_reg + ", " + res_reg + ", 0"); // arg != 0 implies res = 1
      }
      else if(code_arr[i].op == "SIZEOF"){
        string res_reg = get_reg(code_arr[i].res, false);
        asmb_line("li " + res_reg + ", " + code_arr[i].constant);
      }
      else if(code_arr[i].op == "<<"){
        reg1 = get_reg(code_arr[i].arg1);
        reg2 = get_reg(code_arr[i].arg2);
        reg3 = get_reg(code_arr[i].res, false);
        asmb_line("sll " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " << " + code_arr[i].arg2->sym_name);
      }
      else if(code_arr[i].op == ">>"){
        reg1 = get_reg(code_arr[i].arg1);
        reg2 = get_reg(code_arr[i].arg2);
        reg3 = get_reg(code_arr[i].res, false);
        asmb_line("srl " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " >> " + code_arr[i].arg2->sym_name); // Arithmetic or logical shift?
      }
      else if(code_arr[i].op == "^"){
        reg1 = get_reg(code_arr[i].arg1);
        reg2 = get_reg(code_arr[i].arg2);
        reg3 = get_reg(code_arr[i].res, false);
        asmb_line("xor " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " ^ " + code_arr[i].arg2->sym_name);
      }
      else if(code_arr[i].op == "|"){
        reg1 = get_reg(code_arr[i].arg1);
        reg2 = get_reg(code_arr[i].arg2);
        reg3 = get_reg(code_arr[i].res, false);
        asmb_line("or " + reg3 + ", " + reg1 + ", " + reg2 + "\t # "+code_arr[i].res->sym_name + " = "+code_arr[i].arg1->sym_name + " | " + code_arr[i].arg2->sym_name);
      }
    }
    if (global_gen) spill_regs();
  }
  dump_asm_code(link_lib_funcs);
}