#include "codegen.h"
#include "3ac.h"
#include "type_check.h"

extern sym_tab global_sym_tab;
extern map<string, sym_tab*> func_sym_tab_map;
extern map<string, vector<sym_tab_entry*>> func_syms_map;

map <string, qid> reg_to_sym;
map <qid, string> sym_to_place;
queue <string> regs_in_use;
queue <string> fregs_in_use;

string reg1, reg2, reg3;
string curr_func;
int fp_cond = 0;
stack<qid> parameters;

vector <string> assembly_code;

void asmb_header(string s){
  cout << s << endl;
}

void asmb_label(string s){
  // assembly_code.push_back(s);
  cout << "\t" << s << endl;
}

void asmb_line(string s){
  // assembly_code.push_back("\t"+s);
  cout << "\t\t" << s << endl;
}

void dump_asm_code(){
  ofstream out_file("test.asm");
  for(string it : assembly_code) out_file << it << endl;//printf("%s\n", it.c_str());
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
  // f4, f6 -> args

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
  reg_to_sym["$f8"] = NULL;
  reg_to_sym["$f10"] = NULL;
  reg_to_sym["$f12"] = NULL;
  reg_to_sym["$f14"] = NULL;
  reg_to_sym["$f16"] = NULL;
  reg_to_sym["$f18"] = NULL;
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
        if (from_mem) asmb_line("lwc1 " + itr.first + ", " + to_string(-offset) + "($fp)");

        return itr.first;
      }
    }
    // no free reg available
    string reg = fregs_in_use.front();
    fregs_in_use.pop();
    // store val from reg to memory
    offset += reg_to_sym[reg]->offset;
    asmb_line("swc1 " + reg + ", " + to_string(-offset) + "($fp)");
    sym_to_place[reg_to_sym[reg]] = "";
    // load val from memory to reg
    offset -= reg_to_sym[reg]->offset;
    offset += sym->offset;
    if (from_mem) asmb_line("lwc1 " + reg + ", " + to_string(-offset) + "($fp)");
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
            asmb_line("lb " + itr.first + ", " + to_string(-offset) + "($fp)");
          } 
          else {
            asmb_line("lw " + itr.first + ", " + to_string(-offset) + "($fp)");
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
    if(is_type_char(sym->type) || is_type_bool(sym->type)) asmb_line("sb " + reg + ", " + to_string(-offset) + "($fp)");
    else {
      asmb_line("sw " + reg + ", " + to_string(-offset) + "($fp)");
    } 
    sym_to_place[reg_to_sym[reg]] = "";
    // load val from memory to reg
    offset -= reg_to_sym[reg]->offset;
    offset += sym->offset;
    if (from_mem){
      if(is_type_char(sym->type) || is_type_bool(sym->type)){
        asmb_line("move " + reg + ", $zero");
        asmb_line("lb " + reg + ", " + to_string(-offset) + "($fp)");
      } 
      else {
        asmb_line("lw " + reg + ", " + to_string(-offset) + "($fp)");
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
        asmb_line("sb " + itr.first + ", " + to_string(-offset) + "($fp)");
      }
      else if (is_type_float(itr.second->type)){
        asmb_line("swc1 " + itr.first + ", " + to_string(-offset) + "($fp)");
      }
      else {
        asmb_line("sw " + itr.first + ", " + to_string(-offset) + "($fp)");
      }
      offset -= itr.second->offset;
      sym_to_place[itr.second] = "";
      reg_to_sym[itr.first] = NULL;
    }
  }
}

void code_gen(){
    initialise_regs();

    asmb_header(".data\n");
    for (auto itr : global_sym_tab){
      if (itr.second->type.substr(0, 5) != "func "){
        string append = itr.second->sym_name + " :\t .space " + to_string(itr.second->size);
        asmb_label(append);
      }
    }

    asmb_header(".text\n");

    // Store goto label locations to create label in assembly
    unordered_set<int> label_lines;
    for (int i=0; i<code_arr.size(); i++){
      if (code_arr[i].op == "GOTO" || code_arr[i].op == "GOTO IF"){
        int tmp = code_arr[i].goto_label;
        while (code_arr[tmp].op == "GOTO") tmp = code_arr[tmp].goto_label;
        label_lines.insert(tmp);
      }
    }

    for (int i=0; i < (int)code_arr.size(); i++){
      // cout << "op : " << code_arr[i].op << endl;
      if(label_lines.count(i)){
        asmb_label("Label_" + to_string(i) + " :");
      }

      if(code_arr[i].op.substr(0, 14) == "*function end*"){
        if (curr_func == "main"){
          asmb_label("main_end : ");
          asmb_line("li $v0, 10");
          asmb_line("syscall");
        }
        else {
          asmb_label(curr_func + "_end : ");
          asmb_line("lw $fp, " + to_string(FUNC_AR_SIZE-4) + "($sp)");
          asmb_line("lw $ra, " + to_string(FUNC_AR_SIZE-8) + "($sp)");
          asmb_line("addu $sp, $sp, " + to_string(FUNC_AR_SIZE));
          asmb_line("jr $ra");
        }
        initialise_regs();
      }
      else if(code_arr[i].op.substr(0, 16) == "*function start*"){
        // cout << "Assembly for function start\n";
        // Extract the name of the func from op

        curr_func = code_arr[i].op.substr(18);
        curr_func.pop_back();

        if(curr_func == "main"){
          asmb_label("main:");
          asmb_line("subu $sp, $sp, " + to_string(MAIN_AR_SIZE));
          asmb_line("sw $fp, " + to_string(MAIN_AR_SIZE-4) + "($sp)");
          asmb_line("addu $fp, $sp, " + to_string(MAIN_AR_SIZE));
        }
        else{
          asmb_label(curr_func + ":");
          asmb_line("subu $sp, $sp, " + to_string(FUNC_AR_SIZE));
          asmb_line("sw $fp, " + to_string(FUNC_AR_SIZE-4) + "($sp)");
          asmb_line("sw $ra, " + to_string(FUNC_AR_SIZE-8) + "($sp)");
          asmb_line("addu $fp, $sp, " + to_string(FUNC_AR_SIZE));

          // move args to temp regs
          int non_f_args = 0, f_args = 0;
          for (int i=0; i<func_syms_map[curr_func].size(); i++){
            qid sym = (qid)func_syms_map[curr_func][i];
            string reg = get_reg(sym, false);
            if (is_type_float(sym->type)){
              asmb_line("mov.s " + reg + ", $f" + to_string(f_args*2 + 4));
              f_args++;
            }
            else {
              asmb_line("move " + reg + ", $a" + to_string(non_f_args));
              non_f_args++;
            }
          }
        }
      }
      else if(code_arr[i].op == "params"){ // <- params a
          // cout << "Printing params assembly \n";
          parameters.push(code_arr[i].arg2);
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
            asmb_line("mov.s $f" + to_string(f_args*2 + 4) + ", " + reg);
          }
          else {
            non_f_args--;
            asmb_line("move $a" + to_string(non_f_args) + ", " + reg);
          }
        }
        // spill regs
        spill_regs();
        // jump to callee
        asmb_line("jal " + callee);
        if(code_arr[i].res){ // Call to non void function
            string res_reg = get_reg(code_arr[i].res, false);
            if (is_type_float(code_arr[i].res->type)) asmb_line("mov.s " + res_reg + ", $f0");
            else asmb_line("move " + res_reg + ", $v0");
        }
      }
      else if(code_arr[i].op == "RETURN"){
        if(code_arr[i].arg2){ // for non-void return types
          string reg = get_reg(code_arr[i].arg2);
          if (is_type_float(code_arr[i].arg2->type)) asmb_line("mov.s $f0, " + reg);
          else asmb_line("move $v0, " + reg);
        }
        asmb_line("b " + curr_func + "_end"); // Branch to function end directive
      }
      else if(code_arr[i].op == "="){  // res <- = arg2, arg2 could be a constant(num) or a temporary
        // cout << "Printing = assembly\n";
        string res_reg = get_reg(code_arr[i].res, false);
        if(code_arr[i].arg2){
            string arg_reg = get_reg(code_arr[i].arg2); // arg2 is a temporary
            if(is_type_float(code_arr[i].arg2->type)) asmb_line("mov.s " + res_reg + ", " + arg_reg);
            else asmb_line("move " + res_reg + ", " + arg_reg);
        }
        else {
          if(is_type_float(code_arr[i].res->type)) asmb_line("li.s " + res_reg + ", " + code_arr[i].constant);
          else asmb_line("li " + res_reg + ", " + code_arr[i].constant);
        }
      }
      else if(code_arr[i].op == "&"){ // res <- &arg2
        // cout << "Printing unary& assembly\n";
        string res_reg = get_reg(code_arr[i].res, false);
        long long offset = curr_func == "main" ? MAIN_AR_SIZE : FUNC_AR_SIZE;
        offset += code_arr[i].arg2->offset;
        asmb_line("subu " + res_reg + ", $fp, " + to_string(offset));
      }
      else if(code_arr[i].op == "*"){ // res <- *arg2
          // cout << "Printing unary* assembly\n";
          string res_reg = get_reg(code_arr[i].res, false);
          string arg_reg = get_reg(code_arr[i].arg2);
          if (is_type_float(code_arr[i].res->type)) asmb_line("lwc1 " + res_reg + ", 0(" + arg_reg + ")");
          else if (is_type_bool(code_arr[i].res->type) || is_type_char(code_arr[i].res->type)) asmb_line("lb " + res_reg + ", 0(" + arg_reg + ")");
          else asmb_line("lw " + res_reg + ", 0(" + arg_reg + ")");
      }
      else if(code_arr[i].op == "-"){ // res <- -arg2
          // cout << "Printing unary- assembly\n";
          string res_reg = get_reg(code_arr[i].res, false);
          string arg_reg = get_reg(code_arr[i].arg2);
          if(is_type_float(code_arr[i].arg2 -> type)) asmb_line("neg.s " + res_reg + ", " + arg_reg);
          else asmb_line("neg " + res_reg + ", " + arg_reg);
      }
      else if(code_arr[i].op == "+"){ // res <- +arg2
          // cout << "Printing unary+ assembly\n";
          string res_reg = get_reg(code_arr[i].res, false);
          string arg_reg = get_reg(code_arr[i].arg2);
          if (is_type_float(code_arr[i].res->type)) asmb_line("lwc1 " + res_reg + ", " + arg_reg);
          else if (is_type_bool(code_arr[i].res->type) || is_type_char(code_arr[i].res->type)) asmb_line("lb " + res_reg + ", " + arg_reg);
          else asmb_line("lw " + res_reg + ", " + arg_reg);
      }
      else if(code_arr[i].op == "~"){ // res <- ~arg2
          // cout << "Printing unary ~ assembly\n";
          string res_reg = get_reg(code_arr[i].res, false);
          string arg_reg = get_reg(code_arr[i].arg2);
          asmb_line("not " + res_reg + ", " + arg_reg);
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
          asmb_line("seq " + res_reg + ", " + arg_reg + ", " + "$zero");
      }
      // addition
      else if(code_arr[i].op == "+int" || code_arr[i].op == "+float" || code_arr[i].op == "+ptr"){
          // cout << "Printing addition assembly\n";
          string res_reg = get_reg(code_arr[i].res, false);
          string arg1_reg = get_reg(code_arr[i].arg1);
          string arg2_reg = get_reg(code_arr[i].arg2);
          if(code_arr[i].op == "+int") asmb_line("add " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
          else if(code_arr[i].op == "+ptr") asmb_line("addu " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
          else asmb_line("add.s " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
      }
      // subtraction
      else if(code_arr[i].op == "-int" || code_arr[i].op == "-float" || code_arr[i].op == "-ptr"){
          // cout << "Printing subtraction assembly\n";
          string res_reg = get_reg(code_arr[i].res, false);
          string arg1_reg = get_reg(code_arr[i].arg1);
          string arg2_reg = get_reg(code_arr[i].arg2);
          if(code_arr[i].op == "-int") asmb_line("sub " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
          else if(code_arr[i].op == "-ptr") asmb_line("subu " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
          else asmb_line("sub.s " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
      }
      // Integer multiplication
      else if(code_arr[i].op == "*int" || code_arr[i].op == "*float"){
          // cout << "Printing multiplication assembly\n";
          string res_reg = get_reg(code_arr[i].res, false);
          string arg1_reg = get_reg(code_arr[i].arg1);
          if (code_arr[i].arg2){
              string arg2_reg = get_reg(code_arr[i].arg2);
              if(code_arr[i].op == "*int") asmb_line("mul " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
              else if(code_arr[i].op == "*float") asmb_line("mul.s " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
          }
          else{
              if(code_arr[i].op == "*int"){
                asmb_line("li " + res_reg + ", " + code_arr[i].constant);
                asmb_line("mul " + res_reg + ", " + arg1_reg + ", " + res_reg);
              }
              else if(code_arr[i].op == "*float"){
                  asmb_line("li.s " + res_reg + ", " + code_arr[i].constant);
                  asmb_line("mul.s " + res_reg + ", " + arg1_reg + ", " + res_reg);
              }
          }
      }
      // Integer division
      else if(code_arr[i].op == "/int" || code_arr[i].op == "/float"){
        //  cout << "Printing division assembly\n";
          string res_reg = get_reg(code_arr[i].res, false);
          string arg1_reg = get_reg(code_arr[i].arg1);
          string arg2_reg = get_reg(code_arr[i].arg2);
          if(code_arr[i].op == "/int") asmb_line("div " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
          else if(code_arr[i].op == "/float") asmb_line("div.s " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
      }
      // Integer modulo
      else if(code_arr[i].op == "%int"){
          // cout << "Printing modulo assembly\n";
          string res_reg = get_reg(code_arr[i].res, false);
          string arg1_reg = get_reg(code_arr[i].arg1);
          string arg2_reg = get_reg(code_arr[i].arg2);
          asmb_line("div " + arg1_reg +", " + arg2_reg);
          asmb_line("mfhi " + res_reg);
      }
      else if(code_arr[i].op == "<int" || code_arr[i].op == "<float" || code_arr[i].op == "<ptr"){
        reg1 = get_reg(code_arr[i].arg1);
        reg2 = get_reg(code_arr[i].arg2);
        reg3 = get_reg(code_arr[i].res, false);
        if(code_arr[i].op == "<int") asmb_line("slt " + reg3 + ", " + reg1 + ", " + reg2);
        else if(code_arr[i].op == "<ptr") asmb_line("sltu " + reg3 + ", " + reg1 + ", " + reg2);
        else {
          asmb_line("c.lt.s " + reg1 + ", " + reg2);
          asmb_line("bc1t fp_cond_true_" + to_string(fp_cond));
          asmb_line("li " + reg3 + ", 0");
          asmb_line("b fp_cond_end_" + to_string(fp_cond));
          asmb_label("fp_cond_true_" + to_string(fp_cond) + " : ");
          asmb_line("li " + reg3 + ", 1");
          asmb_label("fp_cond_end_" + to_string(fp_cond) + " : ");
          fp_cond++;
        } 
     }
     // >
     else if(code_arr[i].op == ">int" || code_arr[i].op == ">float" || code_arr[i].op == ">ptr"){
        reg1 = get_reg(code_arr[i].arg1);
        reg2 = get_reg(code_arr[i].arg2);
        reg3 = get_reg(code_arr[i].res, false);
        if(code_arr[i].op == ">int") asmb_line("sgt " + reg3 + ", " + reg1 + ", " + reg2);
        else if(code_arr[i].op == ">ptr") asmb_line("sgtu " + reg3 + ", " + reg1 + ", " + reg2);
        else {
          asmb_line("c.le.s " + reg1 + ", " + reg2);
          asmb_line("bc1t fp_cond_true_" + to_string(fp_cond));
          asmb_line("li " + reg3 + ", 1");
          asmb_line("b fp_cond_end_" + to_string(fp_cond));
          asmb_label("fp_cond_true_" + to_string(fp_cond) + " : ");
          asmb_line("li " + reg3 + ", 0");
          asmb_label("fp_cond_end_" + to_string(fp_cond) + " : ");
          fp_cond++;
        }
     }
     // >=
     else if(code_arr[i].op == ">=int" || code_arr[i].op == ">=float" || code_arr[i].op == ">=ptr"){
        reg1 = get_reg(code_arr[i].arg1);
        reg2 = get_reg(code_arr[i].arg2);
        reg3 = get_reg(code_arr[i].res, false);
        if(code_arr[i].op == ">=int") asmb_line("sge " + reg3 + ", " + reg1 + ", " + reg2);
        else if(code_arr[i].op == ">=ptr") asmb_line("sgeu " + reg3 + ", " + reg1 + ", " + reg2);
        else {
          asmb_line("c.lt.s " + reg1 + ", " + reg2);
          asmb_line("bc1t fp_cond_true_" + to_string(fp_cond));
          asmb_line("li " + reg3 + ", 1");
          asmb_line("b fp_cond_end_" + to_string(fp_cond));
          asmb_label("fp_cond_true_" + to_string(fp_cond) + " : ");
          asmb_line("li " + reg3 + ", 0");
          asmb_label("fp_cond_end_" + to_string(fp_cond) + " : ");
          fp_cond++;
        }
     }
    // <=
    else if(code_arr[i].op == "<=int" || code_arr[i].op == "<=float" || code_arr[i].op == "<=ptr"){
       reg1 = get_reg(code_arr[i].arg1);
       reg2 = get_reg(code_arr[i].arg2);
       reg3 = get_reg(code_arr[i].res, false);
       if(code_arr[i].op == "<=int") asmb_line("sle " + reg3 + ", " + reg1 + ", " + reg2);
       else if(code_arr[i].op == "<=ptr") asmb_line("sleu " + reg3 + ", " + reg1 + ", " + reg2);
       else {
          asmb_line("c.le.s " + reg1 + ", " + reg2);
          asmb_line("bc1t fp_cond_true_" + to_string(fp_cond));
          asmb_line("li " + reg3 + ", 0");
          asmb_line("b fp_cond_end_" + to_string(fp_cond));
          asmb_label("fp_cond_true_" + to_string(fp_cond) + " : ");
          asmb_line("li " + reg3 + ", 1");
          asmb_label("fp_cond_end_" + to_string(fp_cond) + " : ");
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
        if(code_arr[i].op == "==int") asmb_line("seq " + reg3 + ", " + reg1 + ", " + code_arr[i].constant);
        else if(code_arr[i].op == "==ptr") asmb_line("sequ " + reg3 + ", " + reg1 + ", " + code_arr[i].constant);
        else {
          asmb_line("c.eq.s " + reg1 + ", " + code_arr[i].constant);
          asmb_line("bc1t fp_cond_true_" + to_string(fp_cond));
          asmb_line("li " + reg3 + ", 0");
          asmb_line("b fp_cond_end_" + to_string(fp_cond));
          asmb_label("fp_cond_true_" + to_string(fp_cond) + " : ");
          asmb_line("li " + reg3 + ", 1");
          asmb_label("fp_cond_end_" + to_string(fp_cond) + " : ");
          fp_cond++;
        }
      }
      else{
        reg1 = get_reg(code_arr[i].arg1);
        reg2 = get_reg(code_arr[i].arg2);
        reg3 = get_reg(code_arr[i].res, false);
        if(code_arr[i].op == "==int") asmb_line("seq " + reg3 + ", " + reg1 + ", " + reg2);
        else if(code_arr[i].op == "==ptr") asmb_line("sequ " + reg3 + ", " + reg1 + ", " + reg2);
        else {
          asmb_line("c.eq.s " + reg1 + ", " + reg2);
          asmb_line("bc1t fp_cond_true_" + to_string(fp_cond));
          asmb_line("li " + reg3 + ", 0");
          asmb_line("b fp_cond_end_" + to_string(fp_cond));
          asmb_label("fp_cond_true_" + to_string(fp_cond) + " : ");
          asmb_line("li " + reg3 + ", 1");
          asmb_label("fp_cond_end_" + to_string(fp_cond) + " : ");
          fp_cond++;
        }
      }
    }
    // !=
    else if(code_arr[i].op == "!=int" || code_arr[i].op == "!=float" || code_arr[i].op == "!=ptr"){
       reg1 = get_reg(code_arr[i].arg1);
       reg2 = get_reg(code_arr[i].arg2);
       reg3 = get_reg(code_arr[i].res, false);
       if(code_arr[i].op == "!=int") asmb_line("sne " + reg3 + ", " + reg1 + ", " + reg2);
       else if(code_arr[i].op == "!=ptr") asmb_line("sneu " + reg3 + ", " + reg1 + ", " + reg2);
       else {
          asmb_line("c.eq.s " + reg1 + ", " + reg2);
          asmb_line("bc1t fp_cond_true_" + to_string(fp_cond));
          asmb_line("li " + reg3 + ", 1");
          asmb_line("b fp_cond_end_" + to_string(fp_cond));
          asmb_label("fp_cond_true_" + to_string(fp_cond) + " : ");
          asmb_line("li " + reg3 + ", 0");
          asmb_label("fp_cond_end_" + to_string(fp_cond) + " : ");
          fp_cond++;
       }
    }
    else if(code_arr[i].op == "E++" || code_arr[i].op == "++E"){ 
      reg1 = get_reg(code_arr[i].res, false);
      reg2 = get_reg(code_arr[i].arg1);
      if(is_type_float(code_arr[i].arg1 -> type)) {
        asmb_line("add.s " + reg1 + ", " + reg2 + ", 1");
      }
      else if (is_type_char(code_arr[i].arg1 -> type)) {
        asmb_line("addu " + reg1 + ", " + reg2 + ", 1");
      }
      else {
        asmb_line("addi " + reg1 + ", " + reg2 + ", 1");
      }
    }
    else if(code_arr[i].op == "E--" || code_arr[i].op == "--E"){ 
      reg1 = get_reg(code_arr[i].res, false);
      reg2 = get_reg(code_arr[i].arg1);
      if(is_type_float(code_arr[i].arg1 -> type)) {
        asmb_line("sub.s " + reg1 + ", " + reg2 + ", 1");
      }
      else if (is_type_char(code_arr[i].arg1 -> type)) {
        asmb_line("subu " + reg1 + ", " + reg2 + ", 1");
      }
      else {
        asmb_line("subi " + reg1 + ", " + reg2 + ", 1");
      }
    }
    else if(code_arr[i].op == "GOTO"){ // GOTO
      asmb_line("j Label_" + to_string(code_arr[i].goto_label));
    }
    else if(code_arr[i].op == "GOTO IF"){ // GOTO IF
      string arg_reg = get_reg(code_arr[i].arg2, false);
      asmb_line("bnez " + arg_reg + ", Label_" + to_string(code_arr[i].goto_label));
    }
    else if(code_arr[i].op == "inttochar"){ // Load only one byte(lower 8 bits)
      string res_reg = get_reg(code_arr[i].res, false);
      string arg_reg = get_reg(code_arr[i].arg2);
      asmb_line("and " + res_reg + ", " + arg_reg + ", 0xFF");
    }
    else if(code_arr[i].op == "floattochar"){
      string res_reg = get_reg(code_arr[i].res, false);
      string arg_reg = get_reg(code_arr[i].arg2);
      asmb_line("cvt.w.s $f0, " + arg_reg); // Float to int
      asmb_line("mfc1 " + res_reg + ", $f0"); // move from float reg to int reg
      asmb_line("and " + res_reg + ", " + res_reg + ", 0xFF");
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
      asmb_line("move " + res_reg + ", " + arg_reg);
    }
    else if(code_arr[i].op == "inttobool" || code_arr[i].op == "chartobool"){
      string res_reg = get_reg(code_arr[i].res, false);
      string arg_reg = get_reg(code_arr[i].arg2);
      asmb_line("sne " + res_reg + ", " + arg_reg + ", 0"); // arg != 0 implies res = 1
    }
    else if(code_arr[i].op == "booltoint" || code_arr[i].op == "booltochar"){
      string res_reg = get_reg(code_arr[i].res, false);
      string arg_reg = get_reg(code_arr[i].arg2);
      asmb_line("sne " + res_reg + ", " + arg_reg + ", 0"); // arg == 1 implies res = 1
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
  }
  dump_asm_code();
}
// scanf printf math string
// initialize global data
// no usage of true label for memory
// currently assuming max 2 float params and max 4 non float params
// global vars not handled
// dont gen code corresponding to global stuff
// remaining ops from 3ac
// can sort and align offsets, for now just aligned but that leads to stack holes
// BIG PROBLEM WITH REGS !!!!