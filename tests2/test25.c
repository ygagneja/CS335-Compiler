//Program to test function calls and type-casting in case of different return types and return values + type-conversions

float func1(){
    int a = 2.2;
    float b = 1.1;
    return a+b;
}

int func2(int a, int b){
    return a+b;
}

void func3(int a, int b){
    int c = a;
    int d = b;
    printi(a);
    printi(b);
}

int main(){
    int x1 = 2.5;
    bool x2 = true;
    float x3 = x1;
    float x4 = x1;
    int x5 = x3;
    char c = x1;
    int c1 = 1;
    float d = 2;
    c1 = func1();
    
    c = c1;
    x1 = c;
    x2 = x1;
    x2 = x3;// int-to-bool conversion possible but reverse not supported

    printi(c1);
    c1 = func2(c1, (int)d);
    func3(func2(3, 5), func2(5, 4));
}