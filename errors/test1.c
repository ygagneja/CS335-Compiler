// Allignment error
int main(){
    int a = 3,b = 5;
    float c = 8.1, d = 1.41;
    bool e = true, f = false;
    int  var1 = 20, var2 = 22; 
    int  *ptr1 = &var1;
    int  *ptr2 = &var2;
    float res1 = a + b / c - d;
    float res2 = e * f;
    bool comp = *ptr1 < *ptr2;
    printf(res1);
    printf(res2);
    printi(&var1);
    printi(ptr1);
    printi(*ptr1);
    printi(var1);
    printi(&var2);
    printi(ptr2);
    printi(*ptr2);
    printi(var2);
    printi(comp); // should be 1 but output is 0

    return 0;
}