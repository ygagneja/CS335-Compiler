#include<bits/stdc++.h>
#include<fstream>

using namespace std;

typedef struct {
    string sym_name;
    unsigned long long size;
    long long offset;
    bool init;
    unsigned long long level;
    unsigned long long level_id;
}sym_tab_entry;

typedef unordered_map<tuple<string, unsigned long long, unsigned long long>, sym_tab_entry*> sym_tab;