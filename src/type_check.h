#include <bits/stdc++.h>

using namespace std;

bool is_type_int(string str);
bool is_type_float(string str);
bool is_type_bool(string str);
bool is_type_char(string str);
bool is_type_ptr(string str);
bool is_type_func(string str);
bool is_type_struct(string str);
bool is_type_structptr(string str);
int get_real_size(string s);
string is_valid(string str1, string str2);
string id_type(string str, unsigned long long level, unsigned long long* level_id);
string postfix_type(string str, string exp_type, int label);
string args_type(string str1, string str2);
string unary_type(char* op, string str);
string cast_type(string str1, string str2);
string mul_type(string str1, string str2, char op);
string add_type(string str1, string str2);
string shift_type(string str1, string str2);
string relat_type(string str1, string str2);
string bit_type(string str1, string str2);
string cond_type(string str1, string str2);
string assign_type(string str1, string str2, string op);