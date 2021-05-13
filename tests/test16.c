// Code to test struct data type as function parameter and return

struct employee {
    int id;
    struct salary {int intern; float money; } sal;
};

void print_info(struct employee* e){
    printi(e->id);
    prints("id employee takes salary : ");
    printf(e->sal.money);
    if (e->sal.intern) prints(", might be an intern\n");
    else prints("\n");
}

struct employee* create_employee(int id, int intern, float money){
    struct employee* e = malloc(sizeof(struct employee));
    e->id = id;
    e->sal.intern = intern;
    e->sal.money = e->sal.intern ? 0 : money;
    return e;
}
  
int main()
{
    struct employee *e, *f;
    e = create_employee(1, true, 123.3);
    f = create_employee(2, false, 3244.0);
    print_info(e);
    print_info(f);  
    return 0;
}