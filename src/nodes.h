typedef struct {
    int id;
    char* label;
    string nodetype;
    bool init;
} node;

void graph_init();
void graph_end();
node* terminal(char* label);
node* non_terminal(int case_no, char* label, node* n1 = NULL, node* n2 = NULL, node* n3 = NULL, node* n4 = NULL, node* n5 = NULL, char* op1 = NULL, char* op2 = NULL, char* op3 = NULL);