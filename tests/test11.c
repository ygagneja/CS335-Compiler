//Program to test function calls and type-casting in case of different return types and return values.

float func1(){
    int a = 2, b = 1;
    return a+b;
}

int func2(int a, int b){
    return a+b;
}

void func3(int a, int b){
    int c = a;
    int d = b;
}

int main(){
    int c = 1;
    float d = 2;
    c = func1();
    c = func2(c, d);
    func3(c, d);
}