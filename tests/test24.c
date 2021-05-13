// test for scoping

struct node {
    int val;
    void* next;
};

struct vertex {
    int val;
    float arr[10];
};

int glob = 2;
float glob2 = glob*glob; // yes, expression inits are possible in global vars, happens in main during runtime

int main(){
    struct vertex {
        int data;
        float arr[5];
    };
    struct node { int x; } p;
    struct vertex v;
    int x = 1;
    // printi(p.val); // should throw error
    printi(sizeof(struct node)); // should be 4
    // printi(v.val); // should throw error
    printi(sizeof(struct vertex)); // should be 24
    {
        float x = 2.3;
        printf(x);
    }
    {
        printf(glob2);
        {
            int glob2 = 10;
            printi(glob2);
        }
    }
    prints("\n");
    return 0;
}