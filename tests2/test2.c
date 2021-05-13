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

    a += a+i*b-a;
    printi(a); // should print 28
    a <<= 1;
    printi(a); // should print 56
    a >>= 3;
    printi(a); // should print 7
    a ^= 3;
    printi(a); // should print 4
    a &= 3;
    printi(a); // should print 0
    a |= 3;
    printi(a); // should print 3
    a %= 3;
    printi(a); // should print 0
    a += 10;
    a /= 3; 
    printi(a); // should print 3 
    a -= 3;
    printi(a); // should print 0

    printi(!a); // should print 1
    printi(~a); // should print -1
    printf(res1); // 2.207
    printf(res2); // 0
    printi(comp); // 1
    prints("\n");
    return 0;
}