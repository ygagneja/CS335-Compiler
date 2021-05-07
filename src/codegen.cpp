#include "codegen.h"
#include "3ac.h"
#include "type_check.h"
#include "sym_table.h"

extern sym_tab global_sym_tab;
extern map<string, sym_tab*> func_sym_tab_map;

map <string, qid> reg_to_sym;
map <qid, pair<string, bool>> sym_to_place;
queue <string> regs_in_use;
queue <string> fregs_in_use;

string reg1, reg2, reg3;
int i_n_param = 0, f_n_param = 0; // keep track of the number of params when a func is called inside main
string func_name;

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
    sym_to_place[itr.second] = {"", true};
  }
  for (auto table : func_sym_tab_map){
    for (auto itr : *(table.second)){
      sym_to_place[itr.second] = {"", true};
    }
  }

  // f0 and f2 -> return values
  // f4, f6, f8, f10 -> temp regs preseved by callee
  // f12, f14 -> args
  // f16, f18 -> more temp regs, not preserved by callee
  // f20, f22, f24, f26, f28, f30 -> saved regs, prserved by caller

  // We will use the convention that f16, f18 will be used for calcs
  // Similary for occassional storing into memory will use the regs f28, f30(akin to s6, s7 in int case)

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
  reg_to_sym["$t0"] = NULL;
  reg_to_sym["$t1"] = NULL;
  reg_to_sym["$f0"] = NULL;
  reg_to_sym["$f1"] = NULL;
  reg_to_sym["$f2"] = NULL;
  reg_to_sym["$f3"] = NULL;
  reg_to_sym["$f4"] = NULL;
  reg_to_sym["$f5"] = NULL;
  reg_to_sym["$f6"] = NULL;
  reg_to_sym["$f7"] = NULL;
  reg_to_sym["$f8"] = NULL;
  reg_to_sym["$f9"] = NULL;
  reg_to_sym["$f10"] = NULL;
  reg_to_sym["$f11"] = NULL;
  reg_to_sym["$f12"] = NULL;
  reg_to_sym["$f13"] = NULL;
  reg_to_sym["$f14"] = NULL;
  reg_to_sym["$f15"] = NULL;
  reg_to_sym["$f16"] = NULL;
  reg_to_sym["$f17"] = NULL;
  reg_to_sym["$f18"] = NULL;
  reg_to_sym["$f19"] = NULL;
  reg_to_sym["$f20"] = NULL;
  reg_to_sym["$f21"] = NULL;
  reg_to_sym["$f22"] = NULL;
  reg_to_sym["$f23"] = NULL;
  reg_to_sym["$f24"] = NULL;
  reg_to_sym["$f25"] = NULL;
  reg_to_sym["$f26"] = NULL;
  reg_to_sym["$f27"] = NULL;
  reg_to_sym["$f28"] = NULL;
  reg_to_sym["$f29"] = NULL;
  reg_to_sym["$f30"] = NULL;
  reg_to_sym["$f31"] = NULL;
}

string get_reg(qid sym){
  // cout << t -> sym_name << " " << t -> offset << endl;
  if (sym_to_place[sym].first != ""){
    return sym_to_place[sym].first;
  }

  if (is_type_float(sym->type)){
    for (auto itr : reg_to_sym){
      long long offset = func_name == "main" ? MAIN_AR_SIZE : FUNC_AR_SIZE;
      if (itr.first.substr(0, 2) == "$f" && itr.second == NULL){
        // free reg available
        fregs_in_use.push(itr.first);
        itr.second = sym;
        sym_to_place[sym].first = itr.first;
        // load val from memory to reg
        offset += sym->offset;
        asmb_line("lwc1 " + itr.first + " , " + to_string(-offset) + "($fp)");
        
        return itr.first;
      }
      // no free reg available
      string reg = fregs_in_use.front();
      fregs_in_use.pop();
      // store val from reg to memory
      offset += reg_to_sym[reg]->offset;
      asmb_line("swc1 " + reg + " , " + to_string(-offset) + "($fp)");
      sym_to_place[reg_to_sym[reg]].first = "";
      // load val from memory to reg
      offset -= reg_to_sym[reg]->offset;
      offset += sym->offset;
      asmb_line("lwc1 " + reg + " , " + to_string(-offset) + "($fp)");
      sym_to_place[sym].first = reg;
      fregs_in_use.push(reg);
      reg_to_sym[reg] = sym;
      return reg;
    }
  }
  else {
    for (auto itr : reg_to_sym){
      long long offset = func_name == "main" ? MAIN_AR_SIZE : FUNC_AR_SIZE;
      if (itr.first.substr(0, 2) != "$f" && itr.second == NULL){
        // free reg available
        regs_in_use.push(itr.first);
        itr.second = sym;
        sym_to_place[sym].first = itr.first;
        // load val from memory to reg
        offset += sym->offset;
        asmb_line("lw " + itr.first + " , " + to_string(-offset) + "($fp)");
        
        return itr.first;
      }
      // no free reg available
      string reg = regs_in_use.front();
      regs_in_use.pop();
      // store val from reg to memory
      offset += reg_to_sym[reg]->offset;
      asmb_line("sw " + reg + " , " + to_string(-offset) + "($fp)");
      sym_to_place[reg_to_sym[reg]].first = "";
      // load val from memory to reg
      offset -= reg_to_sym[reg]->offset;
      offset += sym->offset;
      asmb_line("lw " + reg + " , " + to_string(-offset) + "($fp)");
      sym_to_place[sym].first = reg;
      regs_in_use.push(reg);
      reg_to_sym[reg] = sym;
      return reg;
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

    for (int i=0; i<code_arr.size(); i++){
      // cout << "op : " << code_arr[i].op << endl;
      if(label_lines.count(i)){
        asmb_label("Label " + to_string(i) + " :\n");
      }

      if(code_arr[i].op.substr(0, 17) == "*function start*"){
        // cout << "Assembly for function start\n";
        // Extract the name of the func from op
        
        func_name = code_arr[i].op.substr(18);
        func_name.pop_back();

        if(func_name == "main"){
            asmb_label("main:");
            asmb_line("move $fp, $sp");
            // func_size = get_func_size(func);
            func_size = 256; // Assumed
            asmb_line("sub $sp, $sp, " + to_string(func_size)); // Decrease the stack pointer by size
        }
        else{
            // func_size = get_func_size(func);
            func_size = 256; // Assumed
            // Set up Activation Record
            asmb_label(func + ":");
            asmb_line("move $fp, $sp");
            asmb_line("sub $sp, $sp, " + to_string(72));
            asmb_line("sw $ra, 0($sp)"); // Store return address
            asmb_line("sw $fp, 4($sp)");

            asmb_line("sw $t0, 12($sp)"); // Store regs into mem
            asmb_line("sw $t1, 16($sp)");
            asmb_line("sw $t2, 20($sp)");
            asmb_line("sw $t3, 24($sp)");
            asmb_line("sw $t4, 28($sp)");
            asmb_line("sw $t5, 32($sp)");
            asmb_line("sw $t6, 36($sp)");
            asmb_line("sw $t7, 40($sp)");
            asmb_line("sw $t8, 44($sp)");
            asmb_line("sw $t9, 48($sp)");
            asmb_line("sw $s0, 52($sp)");
            asmb_line("sw $s1, 56($sp)");
            asmb_line("sw $s2, 60($sp)");
            asmb_line("sw $s3, 64($sp)");
            asmb_line("sw $s4, 68($sp)");

            asmb_line("sub $sp, $sp, " + to_string(72)); // More stack for storing floating point regs
            // Saving floating point regs
            asmb_line("swc1 $f4, 72($sp)");
            asmb_line("swc1 $f6, 68($sp)");
            asmb_line("swc1 $f8, 64($sp)");
            asmb_line("swc1 $f10, 60($sp)");
            asmb_line("swc1 $f16, 56($sp)");
            asmb_line("swc1 $f18, 52($sp)");
            asmb_line("swc1 $f20, 48($sp)");
            asmb_line("swc1 $f22, 44($sp)");
            asmb_line("swc1 $f24, 40($sp)");
            asmb_line("swc1 $f26, 36($sp)");
            asmb_line("swc1 $f28, 32($sp)");
            asmb_line("swc1 $f30, 28($sp)");

            // flush();

            // Floats activation records not set up
            asmb_line("addi $sp, $sp, " + to_string(-func_size)); // Allocate space for temporaries which this func may use

            string arguments = get_func_args(func); // Assume less than 4 args(a0 - a3)
            if(!arguments.empty()){
               int i_args = 0, f_args = 0, param_size = 0;
               size_t f = 1;
               string delim = string(","), curr_argument;
               while(true){
                   f = arguments.find_first_of(delim);
                   if(f == string::npos) {
                     curr_argument = arguments.substr(0, f);
                     param_size += get_size(curr_argument, 0, NULL);
                     if(!typecheck(curr_argument)) {
                       i_args++;
                       asmb_line("addi $s7, $fp, " + to_string(-(param_size + 144)));
                       asmb_line("sw $a" + to_string(i_args - 1) + ", 0($s7)");
                     }
                     else if(typecheck(curr_argument)){
                       f_args++;
                       asmb_line("addi $s7, $fp, " + to_string(-(param_size + 144)));
                       asmb_line("swc1 $f" + to_string(12 + 2 * (f_args - 1)) + ", 0($s7)");
                     }
                     break;
                   }
                   else {
                     curr_argument = arguments.substr(0, f), arguments = arguments.substr(f + 1);
                     param_size += get_size(curr_argument, 0, NULL);
                     if(!typecheck(curr_argument)){
                       i_args++;
                       asmb_line("addi $s7, $fp, " + to_string(-(param_size + 144)));
                       asmb_line("sw $a" + to_string(i_args - 1) + ", 0($s7)");
                     }
                    else if(typecheck(curr_argument)){
                        f_args++;
                        asmb_line("addi $s7, $fp, " + to_string(-(param_size + 144)));
                        asmb_line("swc1 $f" + to_string(12 + 2 * (f_args - 1)) + ", 0($s7)");
                    }
                   }
               }
            }
        }
      }
      else if(code_arr[i].op == "params"){ // <- params a
          // cout << "Printing params assembly \n";
          string arg_reg = get_reg(code_arr[i].arg2);
          if(!typecheck(code_arr[i].arg2 -> type)) asmb_line("move $a" + to_string(i_n_param) + " , " + arg_reg), i_n_param++;
          else if(typecheck(code_arr[i].arg2 -> type)) asmb_line("mov.x $f" + to_string(12 + 2 * f_n_param) + " , " + arg_reg), f_n_param++;
      }

      else if(code_arr[i].op == "="){  // res <- = arg2, arg2 could be a constant(num) or a temporary
        // cout << "Printing = assembly\n";
        string res_reg = get_reg(code_arr[i].res);
        if(code_arr[i].arg2){
            string arg_reg = get_reg(code_arr[i].arg2); // arg2 is a temporary
            if(!typecheck(code_arr[i].arg2 -> type)) asmb_line("move " + res_reg + " , " + arg_reg);
            else asmb_line("mov.x " + res_reg + " , " + arg_reg);
        }
        else {
          asmb_line("addi " + res_reg + ", $0, " + code_arr[i].constant); // arg2 constant
          // How to check type of a constant
        }
      }
      else if(code_arr[i].op == "&"){ // res <- &arg2
        // cout << "Printing unary& assembly\n";
        string res_reg = get_reg(code_arr[i].res);
        long long offset = -code_arr[i].arg2 -> offset;
        if(func != "main") offset -= 144; // 144 bc floats ACrecord included
        if(!typecheck(code_arr[i].arg2 -> type)) asmb_line("addi " + res_reg + ", $fp, " + to_string(offset));
        else  {
          asmb_line("li.x " + res_reg + " , " + to_string(offset));
          asmb_line("add.x " + res_reg + " , $fp, " + to_string(offset));
        }
      }
      else if(code_arr[i].op == "*"){ // res <- *arg2
          // cout << "Printing unary* assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg_reg = get_reg(code_arr[i].arg2);
          asmb_line("lw " + res_reg + " ,  0(" + arg_reg + ")");
      }
      else if(code_arr[i].op == "-"){ // res <- -arg2
          // cout << "Printing unary- assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg_reg = get_reg(code_arr[i].arg2);
          if(!typecheck(code_arr[i].arg2 -> type)) asmb_line("neg " + res_reg + " , " + arg_reg);
          else if(typecheck(code_arr[i].arg2 -> type)) asmb_line("neg.x " + res_reg + " , " + arg_reg);
      }
      else if(code_arr[i].op == "+"){ // res <- +arg2
          // cout << "Printing unary+ assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg_reg = get_reg(code_arr[i].arg2);
          if(!typecheck(code_arr[i].arg2 -> type)) asmb_line("move " + res_reg + " , " + arg_reg);
          else if(typecheck(code_arr[i].arg2 -> type)) asmb_line("mov.x " + res_reg + " , " + arg_reg);
      }
      else if(code_arr[i].op == "~"){ // res <- ~arg2
          // cout << "Printing unary ~ assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg_reg = get_reg(code_arr[i].arg2);
          asmb_line("not " + res_reg + " , " + arg_reg);
      }
      else if(code_arr[i].op == "!"){ // res <- !arg2
          // cout << "Printing unary ! assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg_reg = get_reg(code_arr[i].arg2);
          asmb_line("seq " + res_reg + " , " + arg_reg + ", " + "$0");
      }
      else if(code_arr[i].op == "floattoint"){ // res <- floattoint arg2
        // cout << "Printing type conversion floattoint assembly\n";
        string res_reg = get_reg(code_arr[i].res);
        string arg2_reg = get_reg(code_arr[i].arg2);
        asmb_line("cvt.x.w " + res_reg + " , " + arg2_reg);
      }
      else if(code_arr[i].op == "inttofloat"){ // res <- inttofloat arg2
        // cout << "Printing type conversion inttofloat assembly\n";
        string res_reg = get_reg(code_arr[i].res);
        string arg2_reg = get_reg(code_arr[i].arg2);
        asmb_line("cvt.w.x " + res_reg + " , " + arg2_reg);
      }
      // addition
      else if(code_arr[i].op == "+int" || code_arr[i].op == "+float" || code_arr[i].op == "+ptr"){
          // cout << "Printing addition assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg1_reg = get_reg(code_arr[i].arg1);
          string arg2_reg = get_reg(code_arr[i].arg2);
          if(code_arr[i].op == "+int") asmb_line("add " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
          else if(code_arr[i].op == "+ptr") asmb_line("addu " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
          else asmb_line("add.s " + res_reg + " , " + arg1_reg + " , " + arg2_reg);
      }
      // subtraction
      else if(code_arr[i].op == "-int" || code_arr[i].op == "-float" || code_arr[i].op == "-ptr"){
          // cout << "Printing subtraction assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg1_reg = get_reg(code_arr[i].arg1);
          string arg2_reg = get_reg(code_arr[i].arg2);
          if(code_arr[i].op == "-int") asmb_line("sub " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
          else if(code_arr[i].op == "-ptr") asmb_line("subu " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
          else asmb_line("sub.s " + res_reg + " , " + arg1_reg + " , " + arg2_reg);
      }
      // Integer multiplication
      else if(code_arr[i].op == "*int" || code_arr[i].op == "*float"){
          // cout << "Printing multiplication assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg1_reg = get_reg(code_arr[i].arg1);
          if (code_arr[i].arg2){
              string arg2_reg = get_reg(code_arr[i].arg2);
              if(code_arr[i].op == "*int") asmb_line("mul " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
              else if(code_arr[i].op == "*float") asmb_line("mul.s " + res_reg + " , " + arg1_reg + " , " + arg2_reg);
          }
          else{
              if(code_arr[i].op == "*int"){
                asmb_line("addi " + res_reg + ", $0, " + code_arr[i].constant);
                asmb_line("mul " + res_reg + ", " + arg1_reg + ", " + arg1_reg);
              }
              else if(code_arr[i].op == "*float"){
                  asmb_line("li.s " + res_reg + " , " + code_arr[i].constant);
                  asmb_line("mul.s " + res_reg + ", " + arg1_reg + ", " + res_reg);
              }
          }
      }
      // Integer division
      else if(code_arr[i].op == "/int" || code_arr[i].op == "/float"){
        //  cout << "Printing division assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg1_reg = get_reg(code_arr[i].arg1);
          if (code_arr[i].arg2){
            string arg2_reg = get_reg(code_arr[i].arg2);
            if(code_arr[i].op == "/int") asmb_line("mul " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
            else if(code_arr[i].op == "/float") asmb_line("mul.s " + res_reg + " , " + arg1_reg + " , " + arg2_reg);
          }
          // Else shouldn't occur as not in parser
          else{
            if(code_arr[i].op == "/int"){
              asmb_line("addi " + res_reg + ", $0, " + code_arr[i].constant);
              asmb_line("div " + res_reg + ", " + res_reg + ", " + arg1_reg);
            }
            else if(code_arr[i].op == "/float"){
                asmb_line("li.s " + res_reg + " , " + code_arr[i].constant);
                asmb_line("div.s " + res_reg + ", " + arg1_reg + ", " + res_reg);
            }
          }
      }
      // Integer modulo
      else if(code_arr[i].op == "%int"){
          // cout << "Printing modulo assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg1_reg = get_reg(code_arr[i].arg1);
          string arg2_reg = get_reg(code_arr[i].arg2);
          asmb_line("div " + arg1_reg +", " + arg2_reg);
          asmb_line("mfhi " + res_reg);
      }
      else if(code_arr[i].op == "<int" || code_arr[i].op == "<float" || code_arr[i].op == "<ptr"){
        // NOT REQUIRED ( arg2 is never null )
        
        // if(code_arr[i].arg2 == NULL){
        //   if(code_arr[i].op == "<int"){
        //       asmb_line("addi $t9, $0, " + code_arr[i].constant);
        //       reg1 = "$t9";
        //   }
        //   else{
        //       asmb_line("li.s $t9, " + code_arr[i].constant);
        //       reg1 = "$f16";
        //   }
        // }
        // else
        reg1 = get_reg(code_arr[i].arg2);
        reg2 = get_reg(code_arr[i].arg1);
        reg3 = get_reg(code_arr[i].res);
        if(code_arr[i].op == "<int") asmb_line("slt " + reg3 + ", " + reg2 + ", " + reg1);
        else if(code_arr[i].op == "<ptr") asmb_line("sltu " + reg3 + ", " + reg2 + ", " + reg1);
        else asmb_line("c.lt.s " + reg3 + ", " + reg2 + ", " + reg1);
     }

     // >
     else if(code_arr[i].op == ">int" || code_arr[i].op == ">float" || code_arr[i].op == ">ptr"){
        // NOT REQUIRED ( arg2 is never null )
        
        // if(code_arr[i].arg2 == NULL){ // t9 can be used directly as a temp register
        //   if(code_arr[i].op == ">int"){
        //     asmb_line("addi $t9, $0, " + code_arr[i].constant);
        //     reg1 = "$t9";
        //   }
        //   else {
        //     asmb_line("li.s $t, " + code_arr[i].constant);
        //     reg1 = "$f16";
        //   }
        // }
        // else
        reg1 = get_reg(code_arr[i].arg2);
        reg2 = get_reg(code_arr[i].arg1);
        reg3 = get_reg(code_arr[i].res);
        if(code_arr[i].op == ">int") asmb_line("sgt " + reg3 + ", " + reg2 + ", " + reg1);
        else if(code_arr[i].op == ">ptr") asmb_line("sgtu " + reg3 + ", " + reg2 + ", " + reg1);
        else asmb_line("c.gt.s " + reg3 + ", " + reg2 + ", " + reg1);
     }

     // >=
     else if(code_arr[i].op == ">=int" || code_arr[i].op == ">=float" || code_arr[i].op == ">=ptr"){
        // NOT REQUIRED ( arg2 is never null )
        
        // if(code_arr[i].arg2 == NULL){
        //   if(code_arr[i].op == ">=int"){
        //     asmb_line("addi $t9, $0, " + code_arr[i].constant);
        //     reg1 = "$t9";
        //   }
        //   else{
        //     asmb_line("li.s $t9, " + code_arr[i].constant);
        //     reg1 = "$f16";
        //   }
        // }
        // else
        reg1 = get_reg(code_arr[i].arg2);
        reg2 = get_reg(code_arr[i].arg1);
        reg3 = get_reg(code_arr[i].res);
        if(code_arr[i].op == ">=int") asmb_line("sge " + reg3 + ", " + reg2 + ", " + reg1);
        else if(code_arr[i].op == ">=ptr") asmb_line("sgeu " + reg3 + ", " + reg2 + ", " + reg1);
        else asmb_line("c.ge.s " + reg3 + ", " + reg2 + ", " + reg1);
     }

       // <=
    else if(code_arr[i].op == "<=int" || code_arr[i].op == "<=float" || code_arr[i].op == "<=ptr"){
      // NOT REQUIRED ( arg2 is never null )
        
      //  if(code_arr[i].arg2 == NULL){
      //     if(code_arr[i].op == "<=int"){
      //       asmb_line("addi $t9, $0, " + code_arr[i].constant);
      //       reg1 = "$t9";
      //     }
      //     else{
      //       asmb_line("li.s $t9, " + code_arr[i].constant);
      //       reg1 = "$f16";
      //     }
      //  }
      //  else
       reg1 = get_reg(code_arr[i].arg2);
       reg2 = get_reg(code_arr[i].arg1);
       reg3 = get_reg(code_arr[i].res);
       if(code_arr[i].op == "<=int") asmb_line("sle " + reg3 + ", " + reg2 + ", " + reg1);
       else if(code_arr[i].op == "<=ptr") asmb_line("sleu " + reg3 + ", " + reg2 + ", " + reg1);
       else asmb_line("c.le.s " + reg3 + ", " + reg2 + ", " + reg1);
    }

    // ==
    else if(code_arr[i].op == "==int" || code_arr[i].op == "==float" || code_arr[i].op == "==ptr"){
      // cout << "Checking Equality\n";
       if(code_arr[i].arg2 == NULL){
         // arg2 is never NULL for ==ptr (for now)
          if(code_arr[i].op == "==int"){
            asmb_line("addi $t9, $0, " + code_arr[i].constant);
            reg1 = "$t9";
          }
          else {
            asmb_line("li.s $t9, " + code_arr[i].constant);
            reg1 = "$f16";
          }
       }
       else reg1 = get_reg(code_arr[i].arg2);
       reg2 = get_reg(code_arr[i].arg1);
       reg3 = get_reg(code_arr[i].res);
       if(code_arr[i].op == "==int") asmb_line("seq " + reg3 + ", " + reg2 + ", " + reg1);
       else if(code_arr[i].op == "==ptr") asmb_line("sequ " + reg3 + ", " + reg2 + ", " + reg1);
       else asmb_line("c.eq.s " + reg3 + ", " + reg2 + ", " + reg1);
    }

    // !=
    else if(code_arr[i].op == "!=int" || code_arr[i].op == "!=float" || code_arr[i].op == "!=ptr"){
      // NOT REQUIRED ( arg2 is never null )

      //  if(code_arr[i].arg2 == NULL){
      //       if(code_arr[i].op == "!=int"){
      //         asmb_line("addi $t9, $0, " + code_arr[i].constant);
      //         reg1 = "$t9";
      //       }
      //       else {
      //         asmb_line("li.s $t9, " + code_arr[i].constant);
      //         reg1 = "$f16";
      //       }
      //  }
      //  else 
       reg1 = get_reg(code_arr[i].arg2);
       reg2 = get_reg(code_arr[i].arg1);
       reg3 = get_reg(code_arr[i].res);
       if(code_arr[i].op == "!=int") asmb_line("sne " + reg3 + ", " + reg2 + ", " + reg1);
       else if(code_arr[i].op == "!=ptr") asmb_line("sneu " + reg3 + ", " + reg2 + ", " + reg1);
       else asmb_line("c.ne.s " + reg3 + ", " + reg2 + ", " + reg1);
    }

      else if(code_arr[i].op == "call" && code_arr[i].arg2 -> sym_name != "printf"){ // call func_name
        asmb_line("jal " + code_arr[i].arg2 -> sym_name);
        if(code_arr[i].res){ // Call to non void function
            string res_reg = get_reg(code_arr[i].res);
            asmb_line("move " + res_reg + ", $v0");
            i_n_param = 0, f_n_param = 0; // Reset to 0, to be incremented by next func call
        }
        else i_n_param = 0, f_n_param = 0; // Call to function without params, don't do anything
      }
      else if(code_arr[i].op == "call" && code_arr[i].arg2 -> sym_name == "printf"){
        asmb_line("li $v0, 1");
        asmb_line("syscall");
        asmb_line("li $v0, 4");
        asmb_line("la $a0, _newline");
        asmb_line("syscall");
        i_n_param = 0, f_n_param = 0;
      }
      else if(code_arr[i].op == "RETURN" && func == "main"){ // RETURN called in main function
          // cout << "Return from main\n";
          asmb_line("li $a0, 0"); // Assume return value is int

          asmb_line("li $v0, 10");
          asmb_line("syscall"); // Exit
      }
      else if(code_arr[i].op == "RETURN" && func != "main"){
          // cout << "Return from non main\n";
          string arg_reg = get_reg(code_arr[i].arg2);
          asmb_line("move $v0, " + arg_reg);
          asmb_line("b " + func + "_end_"); // Branch to function end directive

          asmb_label(func + "_end_: "); // Assembly for end directive
          asmb_line("addi $sp, $sp, " + to_string(func_size)); // Pop local data

          asmb_line("lw $ra, 0($sp)");
          asmb_line("lw $fp, 4($sp)");
          asmb_line("lw $a0, 8($sp)");

          asmb_line("lw $t0, 12($sp)"); // Restore all the saved registers
          asmb_line("lw $t1, 16($sp)");
          asmb_line("lw $t2, 20($sp)");
          asmb_line("lw $t3, 24($sp)");
          asmb_line("lw $t4, 28($sp)");
          asmb_line("lw $t5, 32($sp)");
          asmb_line("lw $t6, 36($sp)");
          asmb_line("lw $t7, 40($sp)");
          asmb_line("lw $t8, 44($sp)");
          asmb_line("lw $t9, 48($sp)");
          asmb_line("lw $s0, 52($sp)");
          asmb_line("lw $s1, 56($sp)");
          asmb_line("lw $s2, 60($sp)");
          asmb_line("lw $s3, 64($sp)");
          asmb_line("lw $s4, 68($sp)");

          asmb_line("addi $sp, $sp, 72"); // Pop stack
          asmb_line("lwc1 $f4, 72($sp)");
          asmb_line("lwc1 $f6, 68($sp)");
          asmb_line("lwc1 $f8, 64($sp)");
          asmb_line("lwc1 $f10, 60($sp)");
          asmb_line("lwc1 $f16, 56($sp)");
          asmb_line("lwc1 $f18, 52($sp)");
          asmb_line("lwc1 $f20, 48($sp)");
          asmb_line("lwc1 $f22, 44($sp)");
          asmb_line("lwc1 $f24, 40($sp)");
          asmb_line("lwc1 $f26, 36($sp)");
          asmb_line("lwc1 $f28, 32($sp)");
          asmb_line("lwc1 $f30, 28($sp)");

          asmb_line("addi $sp, $sp, 72");
          asmb_line("jr $ra"); // Return to calling procedure
      }
      else if(code_arr[i].op == "E++"){ // res <- i E++
        string res_reg = get_reg(code_arr[i].res);
        string arg_reg = get_reg(code_arr[i].arg1);
        if(!typecheck(code_arr[i].arg1 -> type)) {
          asmb_line("move " + res_reg + " , " + arg_reg);
          asmb_line("addi " + arg_reg + " , " + arg_reg + " , 1");
        }
        else if(typecheck(code_arr[i].arg1 -> type)){
          asmb_line("mov.x " + res_reg + " , " + arg_reg);
          asmb_line("add.x " + arg_reg + " , " + arg_reg + " , 1.0"); // Check this if instruction is valid
        }
      }
      else if(code_arr[i].op == "E--"){ // res <- i E--
        string res_reg = get_reg(code_arr[i].res);
        string arg_reg = get_reg(code_arr[i].arg1);
          if(!typecheck(code_arr[i].arg1 -> type)){
            asmb_line("move " + res_reg + " , " + arg_reg);
            asmb_line("sub " + arg_reg + " , " + arg_reg + " , 1");
          }
          else if(typecheck(code_arr[i].arg1 -> type)){
            asmb_line("move.x " + res_reg + " , " + arg_reg);
            asmb_line("sub.x " + arg_reg + " , " + arg_reg + " , 1.0"); // Check this if instruction is valid
          }
      }
      else if(code_arr[i].op == "--E"){ // res <- --E i
        string res_reg = get_reg(code_arr[i].res);
        string arg_reg = get_reg(code_arr[i].arg2);
        if(!typecheck(code_arr[i].arg2 -> type)){
          asmb_line("sub " + arg_reg + " , " + arg_reg + " , 1");
          asmb_line("move " + res_reg + " , " + arg_reg);
        }
        else if(typecheck(code_arr[i].arg2 -> type)){
          asmb_line("sub.x " + arg_reg + " , " + arg_reg + " , 1.0"); // Check this if instruction is valid
          asmb_line("mov.x " + res_reg + " , " + arg_reg);
        }
      }
      else if(code_arr[i].op == "++E"){ // res <- ++E i
        string res_reg = get_reg(code_arr[i].res);
        string arg_reg = get_reg(code_arr[i].arg2);
        if(!typecheck(code_arr[i].arg2 -> type)){
          asmb_line("addi " + arg_reg + " , " + arg_reg + " , 1");
          asmb_line("move " + res_reg + " , " + arg_reg);
        }
        else if(typecheck(code_arr[i].arg2 -> type)){
          asmb_line("add.x " + arg_reg + " , " + arg_reg + " , 1.0"); // Check this if instruction is valid
          asmb_line("move " + res_reg + " , " + arg_reg);
        }
      }
      else if(code_arr[i].op == "GOTO"){ // GOTO
        asmb_line("j Label" + to_string(code_arr[i].goto_label));
      }
      else if(code_arr[i].op == "GOTO IF"){ // GOTO IF
        string arg_reg = get_reg(code_arr[i].arg2);
        asmb_line("bne $0, " + arg_reg + " , Label" + to_string(code_arr[i].goto_label));
      }

    }
      // dump_asm_code();
}
// res <= "%d\n" not handled
// add these typecasts : inttofloat inttobool inttochar, floattoint floattochar floattobool, chartofloat chartoint chartobool, booltofloat booltoint booltochar
// [ DONE, someone pls review ] add these ops : *int *float, /int /float, %int, +int +float +ptr, -int -float -ptr, >int >float >ptr, <int <float <ptr, >=int >=float >=ptr, <=int <=float <=ptr, ==int ==float ==ptr, !=int !=float !=ptr