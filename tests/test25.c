//Program to test error reporting
struct node{
    int val;
    void* prev;
};
struct node{
    int data;
    void* next;
    float next;
};


int func1(int a, float b);

int func1(int a, int b){
    return (a > b ? a : b);
}

struct node func2(int a){
    struct node tmp;
    tmp.data = 1;
    tmp.next = NULL;
    return tmp;
}

float func3(int a, int b, int c, int d, int e, float f1, float f2, float f3, float f4){
    return a+b+c+d+e+f1+f2+f3+f4;
}

int func4(int a, int a){
    return a;
}

int func5(struct node a){
    return a.data;
}

int func6(int a){
    a = a+1;
}

void func7(){
    return 2;
}

int main(){
    int a = 0, b = 0 , c = 0;
    int ar1[10], ar2[a];
    int ar3[3] = {1, 2, 3};
    int ar4[10] = {1, 2, 3, };
    float f1 = 43.21, a = 1.1;
    char* s1;
    char* s2[10];
    struct node st1;
    struct node *ptr1, *ptr3;
    struct human *ptr2;
    struct node **ptrtoptr;
    bool b1, a;
    void v1, var[10];
    ptr1 = malloc(sizeof(struct node));
    a = IamNotDeclared;
    s1 = "";
    ar1[10] = -1;
    ar1[-10] = -1;
    a = func1();
    a();
    func1(a, ar1);
    fun1(a, a);
    fun1(a);
    func1(a, b, c);
    a(a,b);
    ptr1.data = 1;
    st1.val = 1;
    ptr2 = malloc(sizeof(struct node));
    ptr2->val = -1;
    ptr1->val = 1;
    b1 = true;
    b1++;
    b1--;
    ++b1;
    --b1;
    ptrtoptr = &ptr1;
    ptr3 = *ptrtoptr;
    ptr3=*a;
    ptr1 = -ptr1;
    f1 = 1.12;
    f1 = ~f1;
    ptr1 = !ptr1;
    a = sizeof(struct human);
    *ptr2 = (struct human)a;
    v1 = (void)a;
    ptr1 = (struct node)f1;
    ptr1 = ptr1*5;
    ptr1 = ptr1/5;
    f1 = f1%5;
    ptr1 = ptr1+ptr3;
    ptr1 = ptr1+f1;
    ptr1 = ptr1-ptr3;
    ptr1 = ptr1-f1;
    f1 = f1 << 2;
    f1 = f1 >> 2;
    if(a < v1 || a > v1 || a >= v1 || a <= v1 || a == v1 || a != v1){
        a = 1;
    }
    f1 = f1 & a;
    f1 = f1 ^ a;
    f1 = f1 | a;
    a = (b == 0) ? v1 : b; 
    v1 = 1;
    a = v1;
    func3(1, 2, 3, 4, 5, 1.2, 21.12, 11.11, 32.2);
    func4(1, 2);
    func5(st1);

    switch (a+b*c)
    {
    case 1:{
                a = 1;
                continue;
            }
    case 2:{
                a++;
                break;
            }
        
    case v1:{
                a--;
                break;
            }
    default:
        break;
    }
    inc_a:
        a++;
    if(a < 10){
        goto inc_a;
        continue;
        break;
    }    

}

float func1(int a, float b){
    return a+b;
}