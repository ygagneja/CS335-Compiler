#include "nodes.h"

using namespace std;

extern FILE* ast;
static int id = 0;

void graph_init() {
    fprintf(ast, "digraph G {\n");
    fprintf(ast, "\tordering=out;\n");
}

void graph_end() {
    fprintf(ast, "}\n");
}

node* terminal(char* label){
    node* curr_node = (node*)malloc(sizeof(node));
    curr_node->label = label, curr_node->id = ++id;
    fprintf(ast, "\t%lu [label=\"%s\"];\n", id, label);
    return curr_node;
}

node* non_terminal(int case_no, char* label, node* n1, node* n2, node* n3, node* n4, node* n5, char* op1, char* op2, char* op3){
    node* curr_node = (node*)malloc(sizeof(node));
    curr_node->label = label, curr_node->id = ++id;
    fprintf(ast, "\t%lu [label=\"%s\"];\n", id, label);
    if (case_no == 0){
        int operator_id = ++id;
        if(op1) fprintf(ast, "\t%lu [label=\"%s\"];\n", operator_id, op1);
        if(n1) fprintf(ast, "\t%lu -> %lu;\n", curr_node->id, n1->id);
        if(op1) fprintf(ast, "\t%lu -> %lu;\n", curr_node->id, operator_id);
        if(n2) fprintf(ast, "\t%lu -> %lu;\n", curr_node->id, n2->id);
    }
    else if (case_no == 3){
        if(n1) fprintf(ast, "\t%lu -> %lu;\n", curr_node->id, n1->id);
        if(n2) fprintf(ast, "\t%lu -> %lu;\n", curr_node->id, n2->id);
        if(n3) fprintf(ast, "\t%lu -> %lu;\n", curr_node->id, n3->id);
        if(n4) fprintf(ast, "\t%lu -> %lu;\n", curr_node->id, n4->id);
        if(n5) fprintf(ast, "\t%lu -> %lu;\n", curr_node->id, n5->id);
    }
    return curr_node;
}
