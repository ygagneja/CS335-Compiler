// Fibonacci program to test loops and conditions and recursion

int fib(int a){
    if(a == 1) return 0;
    if(a == 2) return 1;
    return fib(a-1)+fib(a-2);
}

int main() {
    int num;
    int x = 0, y = 1, z = 0;
    int res;
    int i;
    prints("Enter number i to find i^th fibonacci number:");
    num = scani();
    if(num == 1)
        res = 0;
    else
        if(num == 2)
            res = 1;
        else{
            for (i = 2; i < num; i++) {
                z = x + y;
                x = y;
                y = z;
            }
            res = z;
        }
    printi(fib(num));
    printi(res);
    return 0;
}