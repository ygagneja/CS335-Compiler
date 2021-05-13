// Operator Type checking

int func1(int a, int b){
    int c = 10, d = -1;
    return a*c+d;
}

int main(){
    int a = 1, b = 4, c = 1;
    float f1 = 1.1, f2 = 21.2;
    char* str = "Test program";
    char c1 = 'a', c2 = 'b';
    int *ptr1 = &a, *ptr2 = &b;
    bool b1 = true;

    // Binary +, -
    a = a+b;
    f1 = f1+f2;
    ptr1 = ptr1+a;
    ptr1 = ptr1+f1;// Error

    // E++, ++E, E--, --E
    a++;
    f1++;
    ptr1++; // Error
    c1++;
    b1++; // Error

    // assigning address
    ptr1 = &a;
    ptr1 = &ptr2; //Error

    // Dereferencing
    a = *ptr1;
    a = *b; // Error

    // * and /
    a = a*b;
    f1 = f1*a;
    ptr1 = ptr2*2;//Error

    // modulo
    a = a%b;
    f1 = f1%2; // Error

    //Relational ops
    b1 = a < b;
    b1 = f1 < f2;
    b1 = ptr1 < ptr2;
    b1 = ptr1 < f1; //Error

    // bit ops >>, <<, bitwise_and, bitwise_xor, bitwise_or
    a = a >> b;
    f1 = f1 >> 3; //Error
    b1 = b1 << 1;// Error


    //Function call
    func1(a, b);
    a(b, c);// Error

    // integer &, |, ^
    a = a & 21;
    f1 = f1 & 12; // Error
    return 0;
}