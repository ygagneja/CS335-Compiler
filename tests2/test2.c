// Operator Type checking

int main(){
    int a = 3,b = 5, i;
    float c = 8.1, d = 1.41;
    bool e = true, f = false;
    int var1 = 20, var2 = 22; 
    int *ptr1 = &var1;
    int *ptr2 = &var2;
    float res1 = a + b / c - d;
    float res2 = e * f;
    int arr[10];
    bool comp = *ptr1 < *ptr2;
    for(i = 0; i < 10; i++)
        arr[i] = i;
    *(arr+4) = -1;
    ptr1 = arr;
    *(ptr1 + 8) = -1;
    printi(arr[4]);
    printi(arr[8]);

    a += a+b-i*a;
    printi(a);
    a <<= 1;
    printi(a);
    a >>= 3;
    printi(a);
    a ^= 3;
    printi(a);
    a &= 3;
    printi(a);
    a |= 3;
    printi(a);
    a %= 3;
    printi(a);
    a /= 3;
    printi(a);
    a -= 3;
    printi(a);

    printi(!a);
    printi(~a);
    printf(res1);
    printf(res2);
    printi(comp);
    printi(&var1);
    printi(ptr1);
    return 0;
}