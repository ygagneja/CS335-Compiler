// Operator Type checking

void printf(char* str, ...);
void scanf(char* str, ...);

int main(){
    int a = 3,b = 5;
    float c = 8.1, d = 1.41
    bool e = true, f = false;
    int  var1 = 20, var2 = 22; 
    int  *ptr1 = &var1;
    int  *ptr2 = &var2;
    float res1 = a + b / c - d % c;
    float res2 = e * f;
    bool comp = *ptr1 < *ptr2;
    return 0;
}