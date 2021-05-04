#include "codegen.h"
#include "3ac.h"
#include "type_check.h"

string reg1, reg2, reg3;
int i_n_param = 0, f_n_param = 0; // keep track of the number of params when a func is called inside main
queue <pair <string, sym_tab_entry*> > used_regs;
queue <pair <string, sym_tab_entry*> > free_regs;
queue <pair <string, sym_tab_entry*> > f_free_regs;
queue <pair <string, sym_tab_entry*> > f_used_regs;
vector <string> assembly_code;
map <string, sym_tab_entry*> regs; // Map a reg to the allocated symbol table entry pointers
map <string, string> f_regs;
string func;
int func_size;

map <sym_tab_entry*, int> saved;


void asmb_line(string s){
  // assembly_code.push_back("\t"+s);
  cout << "\t" + s << endl;
}

void asmb_label(string s){
  // assembly_code.push_back(s);
  cout << s << endl;
}

void dump_asm_code(){
  for(string it : assembly_code) printf("%s\n", it.c_str());
}

int typecheck(string type){
  if(type == "float" || type == "double" || type == "long double") return 1;
  return 0;
}

// Allocates a reg if available if not then free a used reg
string get_reg(qid t){
    // cout << t -> sym_name << " " << t -> offset << endl;
    int check = 0;
    string alloc_reg;
    for(auto it : regs){
      if(it.second == t) {alloc_reg = it.first, check = 1; break;}
    }
    if(check) return alloc_reg; // register has been allocated for this symbol t
    else{
      if(!typecheck(t -> type)){
      if(free_regs.empty()){
        pair <string, sym_tab_entry*> popped_reg = used_regs.front();
        used_regs.pop();

        // Save the content of popped_reg into memory
        long long offset = (popped_reg.second) -> offset;
        if(func != "main") offset += 148;
        else offset += 4;
        asmb_line("addi $s7, $fp, " + to_string(-offset));
        asmb_line("sw " + popped_reg.first + " , " + "0($s7)");
        saved[popped_reg.second] = 1;

        // Load the fresh value
        offset = t -> offset;
        if(func != "main") offset += 148;
        else offset += 4;
        if(saved[t]){
          asmb_line("addi $s7, $fp, " + to_string(-offset));
          asmb_line("lw " + popped_reg.first + ", 0($s7)");
        }

        regs[popped_reg.first] = t;
        used_regs.push({popped_reg.first, t});
        alloc_reg = popped_reg.first;
      }
      else{
        pair <string, sym_tab_entry*> f = free_regs.front();
        free_regs.pop();
        long long offset = t -> offset;
        if(func != "main") offset += 148;
        else offset += 4;
        if(saved[t]){
          // Load the fresh value from memory
          asmb_line("addi $s7, $fp, " + to_string(-offset));
          asmb_line("lw " + f.first + " , " + "0($s7)");
      }
        regs[f.first] = t;
        used_regs.push({f.first, t});
        alloc_reg = f.first;
      }
    }
    else if(typecheck(t -> type)){
    if(f_free_regs.empty()){
      pair <string, sym_tab_entry*> popped_reg = f_used_regs.front();
      f_used_regs.pop();

      // Save the content of popped_reg into memory
      long long offset = (popped_reg.second) -> offset;
      if(func != "main") offset += 148;
      else offset += 4; // Handle the edge case offset = 0 one should not allocate at the base
      asmb_line("addi $s7, $fp, " + to_string(-offset));
      asmb_line("swc1 " + popped_reg.first + " , " + "0($s7)");
      saved[popped_reg.second] = 1;

      // Load the fresh value
      offset = t -> offset;
      if(func != "main") offset += 148;
      else offset += 4;
      if(saved[t]){
        asmb_line("addi $s7, $fp, " + to_string(-offset));
        asmb_line("lwc1 " + popped_reg.first + ", 0($s7)");
      }

      regs[popped_reg.first] = t;
      f_used_regs.push({popped_reg.first, t});
      alloc_reg = popped_reg.first;
  }
  else{
      pair <string, sym_tab_entry*> f = f_free_regs.front();
      f_free_regs.pop();
      long long offset = t -> offset;
      if(func != "main") offset += 148;
      else offset += 4;
      if(saved[t]){
        // Load the fresh value from memory
        asmb_line("addi $s7, $fp, " + to_string(-offset));
        asmb_line("lwc1 " + f.first + " , " + "0($s7)");
    }
      regs[f.first] = t;
      f_used_regs.push({f.first, t});
      alloc_reg = f.first;
  }
  }
}
    return alloc_reg;
}


void initialise(){
  pair <string, sym_tab_entry*> t0({"$t0", NULL});
  pair <string, sym_tab_entry*> t1({"$t1", NULL});
  pair <string, sym_tab_entry*> t2({"$t2", NULL});
  pair <string, sym_tab_entry*> t3({"$t3", NULL});
  pair <string, sym_tab_entry*> t4({"$t4", NULL});
  pair <string, sym_tab_entry*> t5({"$t5", NULL});
  pair <string, sym_tab_entry*> t6({"$t6", NULL});
  pair <string, sym_tab_entry*> t7({"$t7", NULL});
  pair <string, sym_tab_entry*> t8({"$t8", NULL});
  pair <string, sym_tab_entry*> t9({"$t9", NULL});
  pair <string, sym_tab_entry*> s0({"$s0", NULL});
  pair <string, sym_tab_entry*> s1({"$s1", NULL});
  pair <string, sym_tab_entry*> s2({"$s2", NULL});
  pair <string, sym_tab_entry*> s3({"$s3", NULL});
  pair <string, sym_tab_entry*> s4({"$s4", NULL});
  pair <string, sym_tab_entry*> s5({"$s5", NULL});
  pair <string, sym_tab_entry*> s6({"$s6", NULL});
  pair <string, sym_tab_entry*> s7({"$s7", NULL});
  free_regs.push(t0);
  free_regs.push(t1);
  free_regs.push(t2);
  free_regs.push(t3);
  free_regs.push(t4);
  free_regs.push(t5);
  free_regs.push(t6);
  free_regs.push(t7);
  free_regs.push(t8);
  // free_regs.push(t9); // t9 is used directly
  free_regs.push(s0);
  free_regs.push(s1);
  free_regs.push(s2);
  free_regs.push(s3);
  free_regs.push(s4);

  // free_regs.push(s5); // Regs would be used for storing offsets and performing temporary operations so don't push them to free queue
  // free_regs.push(s6);
  // free_regs.push(s7);
  regs["$t0"] = NULL;
  regs["$t1"] = NULL;
  regs["$t2"] = NULL;
  regs["$t3"] = NULL;
  regs["$t4"] = NULL;
  regs["$t5"] = NULL;
  regs["$t6"] = NULL;
  regs["$t7"] = NULL;
  regs["$t8"] = NULL;
  // regs["$t9"] = NULL;
  regs["$s0"] = NULL;
  regs["$s1"] = NULL;
  regs["$s2"] = NULL;
  regs["$s3"] = NULL;
  regs["$s4"] = NULL;
  regs["$s5"] = NULL;
  regs["$s6"] = NULL;
  regs["$s7"] = NULL;
}

// f0 and f2 -> return values
// f4, f6, f8, f10 -> temp regs preseved by callee
// f12, f14 -> args
// f16, f18 -> more temp regs, not preserved by callee
// f20, f22, f24, f26, f28, f30 -> saved regs, prserved by caller

// We will use the convention that f16, f18 will be used for calcs
// Similary for occassional storing into memory will use the regs f28, f30(akin to s6, s7 in int case)
void initialise_f_regs(){
  pair <string, sym_tab_entry*> f4({"$f4", NULL});
  pair <string, sym_tab_entry*> f6({"$f6", NULL});
  pair <string, sym_tab_entry*> f8({"$f8", NULL});
  pair <string, sym_tab_entry*> f10({"$f10", NULL});
  pair <string, sym_tab_entry*> f20({"$f20", NULL});
  pair <string, sym_tab_entry*> f22({"$f22", NULL});
  pair <string, sym_tab_entry*> f24({"$f24", NULL});
  pair <string, sym_tab_entry*> f26({"$f26", NULL});
  f_free_regs.push(f4);
  f_free_regs.push(f6);
  f_free_regs.push(f8);
  f_free_regs.push(f10);
  f_free_regs.push(f20);
  f_free_regs.push(f22);
  f_free_regs.push(f24);
  f_free_regs.push(f26);
  regs["$f0"] = NULL;
  regs["$f1"] = NULL;
  regs["$f2"] = NULL;
  regs["$f3"] = NULL;
  regs["$f4"] = NULL;
  regs["$f5"] = NULL;
  regs["$f6"] = NULL;
  regs["$f7"] = NULL;
  regs["$f8"] = NULL;
  regs["$f9"] = NULL;
  regs["$f10"] = NULL;
  regs["$f11"] = NULL;
  regs["$f12"] = NULL;
  regs["$f13"] = NULL;
  regs["$f14"] = NULL;
  regs["$f15"] = NULL;
  regs["$f16"] = NULL;
  regs["$f17"] = NULL;
  regs["$f18"] = NULL;
  regs["$f19"] = NULL;
  regs["$f20"] = NULL;
  regs["$f21"] = NULL;
  regs["$f22"] = NULL;
  regs["$f23"] = NULL;
  regs["$f24"] = NULL;
  regs["$f25"] = NULL;
  regs["$f26"] = NULL;
  regs["$f27"] = NULL;
  regs["$f28"] = NULL;
  regs["$f29"] = NULL;
  regs["$f30"] = NULL;
  regs["$f31"] = NULL;
}

void code_gen(){
    asmb_line(".data");
    asmb_line("reservedspace: .space 1024");
    asmb_line("_newline: .asciiz \"\\n\"");
    asmb_line(".text\n");
    initialise();

    // Store goto label locations to create label in assembly
    bool jump_line[code_arr.size()];
    memset(jump_line, false, sizeof(jump_line));
    for(int i = 0; i < (int) code_arr.size(); i++){
      if(code_arr[i].op == "GOTO" || code_arr[i].op == "GOTO IF"){
        int tmp = code_arr[i].goto_label;
        while(code_arr[tmp].op == "GOTO"){
          tmp = code_arr[tmp].goto_label;
        }
        jump_line[tmp] = true;
        code_arr[i].goto_label = tmp;
      }
    }

    initialise_f_regs();
    for(int i = 0; i < (int) code_arr.size(); i++){
      // cout << "op : " << code_arr[i].op << endl;

      if(jump_line[i]){
        asmb_label("Label"+to_string(i)+":");
      }


      if(code_arr[i].op.substr(0, 15) == "*function start"){
        // cout << "Assembly for function start\n";
        // Extract the name of the func from op
        if(!func.empty()) func.clear();
        string tmp = code_arr[i].op;
        int pos = (int) tmp.size() - 1;
        --pos; // skip ')'
        while(tmp[pos] != '(') func += tmp[pos], --pos;
        reverse(func.begin(), func.end());
        if(func == "main"){
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
          else if(typecheck(code_arr[i].arg2 -> type)) asmb_line("mov.x f" + to_string(12 + 2 * f_n_param) + " , " + arg_reg), f_n_param++;
      }

      else if(code_arr[i].op == "="){  // res <- = arg2, arg2 could be a constant(num) or a temporary
        // cout << "Printing = assembly\n";
        string res_reg = get_reg(code_arr[i].res);
        if(code_arr[i].arg2){
            string arg_reg = get_reg(code_arr[i].arg2); // arg2 is a temporary
            if(!typecheck(code_arr[i].arg2 -> type)) asmb_line("move " + res_reg + " , " + arg_reg);
            else asmb_line("mov.x" + res_reg + " , " + arg_reg);
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
          asmb_line("add.x" + res_reg + " , $fp, " + to_string(offset));
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
        asmb_line("cvt.x.w" + res_reg + " , " + arg2_reg);
      }
      else if(code_arr[i].op == "inttofloat"){ // res <- inttofloat arg2
        // cout << "Printing type conversion inttofloat assembly\n";
        string res_reg = get_reg(code_arr[i].res);
        string arg2_reg = get_reg(code_arr[i].arg2);
        asmb_line("cvt.w.x " + res_reg + " , " + arg2_reg);
      }
      // Integer addition
      else if(code_arr[i].op == "+int" || code_arr[i].op == "+float"){
          // cout << "Printing addition assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg1_reg = get_reg(code_arr[i].arg1);
          string arg2_reg = get_reg(code_arr[i].arg2);
          if(code_arr[i].op == "+int") asmb_line("add " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
          else asmb_line("add.s " + res_reg + " , " + arg1_reg + " , " + arg2_reg);
      }
      // Integer subtraction
      else if(code_arr[i].op == "-int" || code_arr[i].op == "-float"){
          // cout << "Printing subtraction assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg1_reg = get_reg(code_arr[i].arg1);
          string arg2_reg = get_reg(code_arr[i].arg2);
          if(code_arr[i].op == "int") asmb_line("sub " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
          else asmb_line("sub.s" + res_reg + " , " + arg1_reg + " , " + arg2_reg);
      }
      // Integer multiplication
      else if(code_arr[i].op == "*int" || code_arr[i].op == "*float"){
          // cout << "Printing multiplication assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg1_reg = get_reg(code_arr[i].arg1);
          if (code_arr[i].arg2){
              string arg2_reg = get_reg(code_arr[i].arg2);
              if(code_arr[i].op == "*int") asmb_line("mul " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
              else if(code_arr[i].op == "*float") asmb_line("mul.s" + res_reg + " , " + arg1_reg + " , " + arg2_reg);
          }
          else{
              if(code_arr[i].op == "*int"){
                asmb_line("addi " + res_reg + ", $0, " + code_arr[i].constant);
                asmb_line("mul " + res_reg + ", " + arg1_reg + ", " + arg1_reg);
              }
              else if(code_arr[i].op == "*float"){
                  asmb_line("li.s" + res_reg + " , " + code_arr[i].constant);
                  asmb_line("mul.s" + res_reg + ", " + arg1_reg + ", " + res_reg);
              }
          }
      }
      // Integer division
      else if(code_arr[i].op == "/int" || code_arr[i].op == "/float"){
         cout << "Printing division assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg1_reg = get_reg(code_arr[i].arg1);
          if (code_arr[i].arg2){
            string arg2_reg = get_reg(code_arr[i].arg2);
            if(code_arr[i].op == "/int") asmb_line("mul " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
            else if(code_arr[i].op == "/float") asmb_line("mul.s" + res_reg + " , " + arg1_reg + " , " + arg2_reg);
          }
          // Else shouldn't occur as not in parser
          else{
            if(code_arr[i].op == "/int"){
              asmb_line("addi " + res_reg + ", $0, " + code_arr[i].constant);
              asmb_line("div " + res_reg + ", " + res_reg + ", " + arg1_reg);
            }
            else if(code_arr[i].op == "/float"){
                asmb_line("li.s" + res_reg + " , " + code_arr[i].constant);
                asmb_line("div.s" + res_reg + ", " + arg1_reg + ", " + res_reg);
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
      else if(code_arr[i].op == "<int" || code_arr[i].op == "<float"){
         if(code_arr[i].arg2 == NULL){
          if(code_arr[i].op == "<int"){
              asmb_line("addi $t9, $0, " + code_arr[i].constant);
              reg1 = "$t9";
            }
          else{
              asmb_line("li.s $t9, " + code_arr[i].constant);
              reg1 = "$f16";
            }
         }
         else reg1 = get_reg(code_arr[i].arg2);
         reg2 = get_reg(code_arr[i].arg1);
         reg3 = get_reg(code_arr[i].res);
         if(code_arr[i].op == "<int") asmb_line("slt " + reg3 + ", " + reg2 + ", " + reg1);
         else asmb_line("c.lt.s " + reg3 + ", " + reg2 + ", " + reg1);
     }

     // >
     else if(code_arr[i].op == ">int" || code_arr[i].op == ">float"){
         if(code_arr[i].arg2 == NULL){ // t9 can be used directly as a temp register
          if(code_arr[i].op == ">int"){
            asmb_line("addi $t9, $0, " + code_arr[i].constant);
            reg1 = "$t9";
          }
          else {
            asmb_line("li.s $t, " + code_arr[i].constant);
            reg1 = "$f16";
          }
         }
         else reg1 = get_reg(code_arr[i].arg2);
         reg2 = get_reg(code_arr[i].arg1);
         reg3 = get_reg(code_arr[i].res);
         if(code_arr[i].op == ">int") asmb_line("sgt " + reg3 + ", " + reg2 + ", " + reg1);
         else asmb_line("c.gt.s " + reg3 + ", " + reg2 + ", " + reg1);
     }

     // >=
     else if(code_arr[i].op == ">=int" || code_arr[i].op == ">=float"){
         if(code_arr[i].arg2 == NULL){
          if(code_arr[i].op == ">=int"){
            asmb_line("addi $t9, $0, " + code_arr[i].constant);
            reg1 = "$t9";
          }
          else{
            asmb_line("li.s $t9, " + code_arr[i].constant);
            reg1 = "$f16";
          }
         }
         else reg1 = get_reg(code_arr[i].arg2);
         reg2 = get_reg(code_arr[i].arg1);
         reg3 = get_reg(code_arr[i].res);
         if(code_arr[i].op == ">=int") asmb_line("sge " + reg3 + ", " + reg2 + ", " + reg1);
         else asmb_line("c.ge.s " + reg3 + ", " + reg2 + ", " + reg1);
     }

       // <=
    else if(code_arr[i].op == "<=int" || code_arr[i].op == "<=float"){
       if(code_arr[i].arg2 == NULL){
          if(code_arr[i].op == "<=int"){
            asmb_line("addi $t9, $0, " + code_arr[i].constant);
            reg1 = "$t9";
          }
          else{
            asmb_line("li.s $t9, " + code_arr[i].constant);
            reg1 = "$f16";
          }
       }
       else reg1 = get_reg(code_arr[i].arg2);
       reg2 = get_reg(code_arr[i].arg1);
       reg3 = get_reg(code_arr[i].res);
       if(code_arr[i].op == "<=int") asmb_line("sle " + reg3 + ", " + reg2 + ", " + reg1);
       else asmb_line("c.le.s " + reg3 + ", " + reg2 + ", " + reg1);
    }

    // ==
    else if(code_arr[i].op == "==int" || code_arr[i].op == "==float"){
      // cout << "Checking Equality\n";
       if(code_arr[i].arg2 == NULL){
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
       else asmb_line("c.eq.s " + reg3 + ", " + reg2 + ", " + reg1);
    }

    // !=
    else if(code_arr[i].op == "!=int" || code_arr[i].op == "!=float"){
       if(code_arr[i].arg2 == NULL){
            if(code_arr[i].op == "!=int"){
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
       if(code_arr[i].op == "!=int") asmb_line("sne " + reg3 + ", " + reg2 + ", " + reg1);
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

          asmb_line(func + "_end_: "); // Assembly for end directive
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
        asmb_line("bneq $0, " + arg_reg + " , Label" + to_string(code_arr[i].goto_label));
      }

    }
      // dump_asm_code();
}
// res <= "%d\n" not handled