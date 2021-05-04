// Fibonacci program to test loops and conditions
void printf(char* str, ...);
void scanf(char* str, ...);

int main() {
    int num = 5;
    int x = 0, y = 1, z = 0;
    int res;
    int i;
    if(num == 1)
    res = 0;
    else if(num == 2)
    res = 1;
    else{
        for (i = 0; i < num; i++) {
            z = x + y;
            x = y;
            y = z;
        }
        res = z;
    }
    printf("%d",res);
    return 0;
}