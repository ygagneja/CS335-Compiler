#include "codegen.h"
#include "3ac.h"
#include "type_check.h"
//#include "sym_table.h"

string reg1, reg2, reg3;
int current_n_param = 0; // keep track of the number of params when a func is called inside main
queue <pair <string, sym_tab_entry*> > used_regs;
queue <pair <string, sym_tab_entry*> > free_regs;
vector <string> assembly_code;
map <string, string> regs; // reg["$s5"] gives the name of the symbol that is allocated to reg $s5

// fstream my_file;
// my_file.open("my_file.S", ios::out);
// 	if (!my_file) {
// 		cout << "File not created!";
// 	}

void asmb_line(string s){
  // assembly_code.push_back(s);
  cout << s << endl;
//   my_file << s << endl;
}

void dump_asm_code(){
  for(string it : assembly_code) printf("%s\n", it.c_str());
}


// Allocates a reg if available
// If not then free a used reg
string get_reg(qid t){
    int check = 0;
    string alloc_reg;
    for(auto it : regs){
      if(it.second == t -> sym_name) {alloc_reg = it.first, check = 1; break;}
    }
    if(check) return alloc_reg; // register has been allocated for this symbol t
    else{
      // cout << "No register has been allocated\n";
      if(free_regs.empty()){
        pair <string, sym_tab_entry*> popped_reg = used_regs.front();
        // cout << "Allocating register " << popped_reg.first << endl;
        used_regs.pop();

        // Save the content of popped_reg into memory
        long long offset = (popped_reg.second) -> offset;
        asmb_line("li $s6, " + to_string(offset));
        asmb_line("sub $s7, $fp, $s6");
        asmb_line("sw " + popped_reg.first + " , " + "0($s7)");

        // Load the fresh value
        offset = t -> offset;
        asmb_line("li $s6, " + to_string(offset));
        asmb_line("sub $s7, $fp, $s6");
        asmb_line("lw " + popped_reg.first + ", 0($s7)");

        regs[popped_reg.first] = t -> sym_name;
        used_regs.push({popped_reg.first, t});
        return popped_reg.first;
      }
      else{
        pair <string, sym_tab_entry*> f = free_regs.front();
        // cout << "Allocating register" << f.first << endl;
        free_regs.pop();
        long long offset = t -> offset;
        // cout << "Offset of the symbol\n is : " << offset << endl;
        // Load the fresh value from memory
        asmb_line("li $s6, " + to_string(offset));
        asmb_line("sub $s7, $fp, $s6");
        asmb_line("lw " + f.first + " , " + "0($s7)");
        regs[f.first] = t -> sym_name;
        used_regs.push({f.first, t});
        return f.first;
      }
    }
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
  free_regs.push(t1);
  free_regs.push(t2);
  free_regs.push(t3);
  free_regs.push(t4);
  free_regs.push(t5);
  free_regs.push(t6);
  free_regs.push(t7);
  free_regs.push(t8);
  free_regs.push(t9);
  free_regs.push(s0);
  free_regs.push(s1);
  free_regs.push(s2);
  free_regs.push(s3);
  free_regs.push(s4);
  free_regs.push(s5);
  free_regs.push(s6);
  free_regs.push(s7);
  regs["$t0"] = "";
  regs["$t1"] = "";
  regs["$t2"] = "";
  regs["$t3"] = "";
  regs["$t4"] = "";
  regs["$t5"] = "";
  regs["$t6"] = "";
  regs["$t7"] = "";
  regs["$t8"] = "";
  // regs["$t9"] = "";
  regs["$s0"] = "";
  regs["$s1"] = "";
  regs["$s2"] = "";
  regs["$s3"] = "";
  regs["$s4"] = "";
  regs["$s5"] = "";
  regs["$s6"] = "";
  regs["$s7"] = "";
}

void code_gen(){
    asmb_line(".data");
    asmb_line("reservedspace: .space 1024");
    asmb_line("_newline: .asciiz \"\\n\"");
    asmb_line(".text\n");
    initialise();
    string func;
    unsigned long long func_size;
    for(int i = 0; i < (int) code_arr.size(); i++){
      cout << "op : " << code_arr[i].op << endl;
      if(code_arr[i].op.substr(0, 15) == "*function start"){
        cout << "Assembly for function start\n";
        // Extract the name of the func from op
        if(!func.empty()) func.clear();
        string tmp = code_arr[i].op;
        int pos = (int) tmp.size() - 1;
        --pos; // skip ')'
        while(tmp[pos] != '(') func += tmp[pos], --pos;
        reverse(func.begin(), func.end());
        if(func == "main"){
            asmb_line("main: la, $fp, ($sp)");
            // func_size = get_func_size(func);
            func_size = 256; // Assumed
            asmb_line("sub $sp, $sp, " + to_string(func_size)); // Decrease the stack pointer by size
        }
        else{
            // func_size = get_func_size(func);
            func_size = 256; // Assumed
            asmb_line(func + " : sub $sp, $sp, " + to_string(72)); // Label of calle function
            asmb_line("sw $ra, 0($sp)"); // Store return address
            asmb_line("sw $fp, 4($sp)");

            asmb_line("la $fp, 72($sp)"); // set frame pointer to basse of alloted stack frame
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

            asmb_line("li $v0, " + to_string(func_size));
            asmb_line("sub $sp, $sp, $v0"); // Allocate space for temporaries which this func may use

            string arguments = get_func_args(func); // Assume less than 4 args(a0 - a3)
            if(!arguments.empty()){
               int n_args = 0, param_size = 0;
               size_t f = 1;
               string delim = string(","), curr_argument;
               while(f != string::npos){
                   n_args++;
                   f = arguments.find_first_of(delim);
                   if(f == string::npos) ;
                   else {
                     curr_argument = arguments.substr(0, f), arguments = arguments.substr(f + 1);
                     asmb_line("li $s6, " + to_string(param_size));
                     asmb_line("sub $s7, $fp, $s6");
                     asmb_line("sw $a" + to_string(n_args - 1) + ", 0($s7)");
                     param_size += get_size(curr_argument, 0, NULL); // default assumed as int, type change later
                   }
               }
            }
        }
      }
      else if(code_arr[i].op == "params"){ // <- params a
          cout << "Printing params assembly \n";
          string arg_reg = get_reg(code_arr[i].arg2);
          asmb_line("move $a" + to_string(current_n_param) + " , " + arg_reg);
          current_n_param++; // Assume less than 4 args
      }
      else if(code_arr[i].op == "="){  // res <- = arg2, arg2 could be a constant(num) or a temporary
        cout << "Printing = assembly\n";
        string res_reg = get_reg(code_arr[i].res);
        if(code_arr[i].arg2){
            string arg_reg = get_reg(code_arr[i].arg2); // arg2 is a temporary
            asmb_line("move " + res_reg + " , " + arg_reg);
        }
        else asmb_line("addi " + res_reg + ", $0, " + code_arr[i].constant); // arg2 constant
      }
      else if(code_arr[i].op == "&"){ // res <- &arg2
        cout << "Printing unary& assembly\n";
        string res_reg = get_reg(code_arr[i].res);
        long long offset = -code_arr[i].arg2 -> offset;
        asmb_line("addi " + res_reg + "$fp, " + to_string(offset));
      }
      else if(code_arr[i].op == "*"){ // res <- *arg2
          cout << "Printing unary* assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg_reg = get_reg(code_arr[i].arg2);
          asmb_line("lw " + res_reg + " ,  0(" + arg_reg + ")");
      }
      else if(code_arr[i].op == "-"){ // res <- -arg2
          cout << "Printing unary- assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg_reg = get_reg(code_arr[i].arg2);
          asmb_line("neg " + res_reg + " , " + arg_reg);
      }
      else if(code_arr[i].op == "+"){ // res <- +arg2
          cout << "Printing unary+ assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg_reg = get_reg(code_arr[i].arg2);
          asmb_line("neg " + res_reg + " , " + arg_reg);
      }
      else if(code_arr[i].op == "~"){ // res <- ~arg2
          cout << "Printing unary ~ assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg_reg = get_reg(code_arr[i].arg2);
          asmb_line("not " + res_reg + " , " + arg_reg);
      }
      else if(code_arr[i].op == "!"){ // res <- !arg2
          cout << "Printing unary ! assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg_reg = get_reg(code_arr[i].arg2);
          asmb_line("not " + res_reg + " , " + arg_reg);
      }
      else if(code_arr[i].op == "floattoint"){ // res <- floattoint arg2
        cout << "Printing type conversion floattoint assembly\n";
        // TODO
      }
      else if(code_arr[i].op == "inttofloat"){ // res <- inttofloat arg2
        cout << "Printing type conversion inttofloat assembly\n";
        // TODO
      }
      // Integer addition
      else if(code_arr[i].op == "+int"){
          cout << "Printing +int assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg1_reg = get_reg(code_arr[i].arg1);
          string arg2_reg = get_reg(code_arr[i].arg2);
          asmb_line("add " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
      }
      // Integer subtraction
      else if(code_arr[i].op == "-int"){
          cout << "Printing -int assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg1_reg = get_reg(code_arr[i].arg1);
          string arg2_reg = get_reg(code_arr[i].arg2);
          asmb_line("sub " + res_reg + ", " + arg1_reg + ", " + arg2_reg);
      }
      // Integer multiplication
      else if(code_arr[i].op == "*int"){
          cout << "Printing *int assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg1_reg = get_reg(code_arr[i].arg1);
          if (code_arr[i].arg2 != NULL){
              string arg2_reg = get_reg(code_arr[i].arg2);
              asmb_line("mult " + arg1_reg + ", " + arg2_reg);
              asmb_line("mflo " + res_reg);
          }
          else{
              asmb_line("addi " + res_reg + ", $0, " + code_arr[i].constant);
              asmb_line("mult " + arg1_reg + ", " + res_reg);
              asmb_line("mflo " + res_reg);
          }
      }
      // Integer division
      else if(code_arr[i].op == "/int"){
         cout << "Printing /int assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg1_reg = get_reg(code_arr[i].arg1);
          if (code_arr[i].arg2 != NULL){
              string arg2_reg = get_reg(code_arr[i].arg2);
              asmb_line("div " + arg1_reg + ", " + arg2_reg);
              asmb_line("mflo " + res_reg);
          }
          // Else shouldn't occur as not in parser
          else{
              asmb_line("addi " + res_reg + ", $0, " + code_arr[i].constant);
              asmb_line("div " + arg1_reg + ", " + res_reg);
              asmb_line("mflo " + res_reg);
          }
      }
      // Integer modulo
      else if(code_arr[i].op == "%int"){
          cout << "Printing modulo assembly\n";
          string res_reg = get_reg(code_arr[i].res);
          string arg1_reg = get_reg(code_arr[i].arg1);
          string arg2_reg = get_reg(code_arr[i].arg2);
          asmb_line("div " + arg1_reg +", " + arg2_reg);
          asmb_line("mfhi " + res_reg);
      }

        // <
     else if(code_arr[i].op == "<int"){
         if(code_arr[i].arg2 == NULL){
             asmb_line("addi $t9, $0, " + code_arr[i].constant);
             reg1 = "$t9";
         }
         else reg1 = get_reg(code_arr[i].arg2);
         reg2 = get_reg(code_arr[i].arg1);
         reg3 = get_reg(code_arr[i].res);
         asmb_line("slt " + reg3 + ", " + reg2 + ", " + reg1);
     }

     // >
     else if(code_arr[i].op == ">int"){
         if(code_arr[i].arg2 == NULL){
             asmb_line("addi $t9, $0, " + code_arr[i].constant);
             reg1 = "$t9";
         }
         else
             reg1 = get_reg(code_arr[i].arg2);
         reg2 = get_reg(code_arr[i].arg1);
         reg3 = get_reg(code_arr[i].res);
         asmb_line("sgt " + reg3 + ", " + reg2 + ", " + reg1);
     }

     // >=
     else if(code_arr[i].op == ">=int"){
         if(code_arr[i].arg2 == NULL){
             asmb_line("addi $t9, $0, " + code_arr[i].constant);
             reg1 = "$t9";
         }
         else reg1 = get_reg(code_arr[i].arg2);
         reg2 = get_reg(code_arr[i].arg1);
         reg3 = get_reg(code_arr[i].res);
         asmb_line("sge " + reg3 + ", " + reg2 + ", " + reg1);
     }

       // <=
    else if(code_arr[i].op == "<=int"){
       if(code_arr[i].arg2 == NULL){
           asmb_line("addi $t9, $0, " + code_arr[i].constant);
           reg1 = "$t9";
       }
       else reg1 = get_reg(code_arr[i].arg2);
       reg2 = get_reg(code_arr[i].arg1);
       reg3 = get_reg(code_arr[i].res);
       asmb_line("sle " + reg3 + ", " + reg2 + ", " + reg1);
    }

    // ==
    else if(code_arr[i].op == "==int"){
       if(code_arr[i].arg2 == NULL){
           asmb_line("addi $t9, $0, " + code_arr[i].constant);
           reg1 = "$t9";
       }
       else reg1 = get_reg(code_arr[i].arg2);
       reg2 = get_reg(code_arr[i].arg1);
       reg3 = get_reg(code_arr[i].res);
       asmb_line("seq " + reg3 + ", " + reg2 + ", " + reg1);
    }

    // !=
    else if(code_arr[i].op == "!=int"){
       if(code_arr[i].arg2 == NULL){
           asmb_line("addi $t9, $0, " + code_arr[i].constant);
           reg1 = "$t9";
       }
       else reg1 = get_reg(code_arr[i].arg2);
       reg2 = get_reg(code_arr[i].arg1);
       reg3 = get_reg(code_arr[i].res);
       asmb_line("sne " + reg3 + ", " + reg2 + ", " + reg1);
    }
      else if(code_arr[i].op == "call" && code_arr[i].arg2 -> sym_name != "printf"){ // call func_name
        asmb_line("jal " + code_arr[i].arg2 -> sym_name);
        if(code_arr[i].res){ // Call to non void function
            string res_reg = get_reg(code_arr[i].res);
            asmb_line("move " + res_reg + ", $v0");
            current_n_param = 0; // Reset to 0, to be incremented by next func call
        }
        else current_n_param = 0; // Call to function without params, don't do anything
      }
      else if(code_arr[i].op == "call" && code_arr[i].arg2 -> sym_name == "printf"){
        asmb_line("li $v0, 1");
        asmb_line("syscall");
        asmb_line("li $v0, 4");
        asmb_line("la $a0, _newline");
        asmb_line("syscall");
        current_n_param = 0;
      }
      else if(code_arr[i].op == "RETURN" && func == "main"){ // RETURN called in main function
          cout << "Return from main\n";
          asmb_line("li $a0, 0");

          asmb_line("li $v0, 10");
          asmb_line("syscall"); // Exit
      }
      else if(code_arr[i].op == "RETURN" && func != "main"){
          cout << "Return from non main\n";
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
          asmb_line("jr $ra"); // Return to calling procedure
      }
        else if(code_arr[i].op == "E++"){ // res <- i E++
          string res_reg = get_reg(code_arr[i].res);
          string arg_reg = get_reg(code_arr[i].arg1);
          asmb_line("move " + res_reg + " , " + arg_reg);
          asmb_line("addi " + arg_reg + " , " + arg_reg + " , 1");
        }
        else if(code_arr[i].op == "E--"){ // res <- i E--
          string res_reg = get_reg(code_arr[i].res);
          string arg_reg = get_reg(code_arr[i].arg1);
          asmb_line("move " + res_reg + " , " + arg_reg);
          asmb_line("sub " + arg_reg + " , " + arg_reg + " , 1");
        }
        else if(code_arr[i].op == "--E"){ // res <- --E i
          string res_reg = get_reg(code_arr[i].res);
          string arg_reg = get_reg(code_arr[i].arg2);
          asmb_line("sub " + arg_reg + " , " + arg_reg + " , 1");
          asmb_line("move " + res_reg + " , " + arg_reg);
        }
        else if(code_arr[i].op == "++E"){ // res <- ++E i
          string res_reg = get_reg(code_arr[i].res);
          string arg_reg = get_reg(code_arr[i].arg2);
          asmb_line("addi " + arg_reg + " , " + arg_reg + " , 1");
          asmb_line("move " + res_reg + " , " + arg_reg);
        }
        else if(code_arr[i].op == "GOTO"){ // GOTO
			asmb_line("j " + to_string(code_arr[i].goto_label));
        }
        else if(code_arr[i].op == "GOTO IF"){ // GOTO IF
            string arg_reg = get_reg(code_arr[i].arg2);
			asmb_line("bnq $0, " + arg_reg + " , " + to_string(code_arr[i].goto_label));
        }

    }
          // dump_asm_code();
	
	
// 	cout << "File dumped successfully!" << endl;
// 	my_file.close(); 
	
}

// +float , *float etc. not handled
// GOTO to be handled
// res <= "%d\n" not handled