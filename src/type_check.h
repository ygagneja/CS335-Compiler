#include <bits/stdc++.h>

using namespace std;

bool is_type_int(string str);
bool is_type_float(string str);
string id_type(string str);
string const_type(string str);
string postfix_type(string str, int label);
string args_type(string str1, string str2);
string unary_type(char* op, string str);
string mul_type(string str1, string str2, char op);