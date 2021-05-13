// Miscellaneous (typecasting + conditional exp)


int main(int i){
    int a = 10000;
    char b = 'a';
    bool c = true;
    float d = 34556.34;
    int res;
    void* ptr;
    int x = 4, y = 5, z = 10;

    printi((char)a);
    printi((bool)a);
    printf((float)a);
    ptr = a;
    printi(ptr);
    printi((int)b);
    printi((bool)b);
    printf((float)b);
    // ptr = b;
    // printi(ptr);
    printi((char)c);
    printi((int)c);
    printf((float)c);
    // ptr = c;
    // printi(ptr);
    printi((char)d);
    printi((bool)d);
    printi((int)d);
    // ptr = d;
    // printi(ptr);
    prints("\n");

    res = (char)a == b ? 10 : d > a ? 100 : 1000;
    printi(res);
    prints("\n");
    y = x==1 ? z+++x : z*(x++)/(++y);
    printi(y); // 6
    y = z+++x;
    printi(y); // 15
    prints("\n");
    return 0;
}