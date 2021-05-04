 
// Program to check all kinds of errors handled

int foo(int a, int b){
    a = 1;
    b = 1;
    b = i;
    return a;
}

int tmp(int a, int b);

int tmp2(){
    return;
}

void tmp3(){
    tmp(1, 2);
    tmp2();
    return 11;
}

int main(){
    int b;
    int b = 1, i;
    bool c = true;
    void d;
    char arr[-10];
    int arr[50];
    b = a;
    arr[2.5] = 0;
    foo(b);
    foo();
    foo(a, b, 5);
    goto uv;
    b();

    uv:
        b = 1;
    
    c++;
    ++c;
    i = (float)b;
    i = 3<<2.5;
    i = 2.5<<3;
    d = 1;

    switch (1)
    {
        case 1:
            continue;
            break;
        
        case 2:
            a = 10;
            break;
    }

    goto uv;
    

}

bool foo(int a, int b){
    return -1;
}

bool tmp(float c, float d){
    return -1;
}
