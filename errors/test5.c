//segfault?

bool chk = false;

struct node
{
    int data;
    float val;
};

void comp(struct node* node1, struct node* node2){
    chk = node1->data >= node2->data ? node1->val < node2->val : node1->val == node2->val;
    return;
}

int main(){
    struct node node1, node2;
    node1.data = 1;
    node1.val = 2.5;
    node2.data = 3;
    node2.val = 5.5;
    comp(&node1, &node2);
    printi(chk);
}