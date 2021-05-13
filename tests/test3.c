// Fibonacci program to test loops and conditions and recursion

int fib(int n){
    if(n == 1) return 0;
    if(n == 2) return 1;
    return fib(n-1)+fib(n-2);
}

int main() {
    int num;
    int x = 0, y = 1, z = 0;
    int res;
    int i;
    prints("Enter number n to find n^th fibonacci number:\n");
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
    prints("Output by iteration method : ");
    printi(res);
    prints("\nOutput by recursive method : ");
    printi(fib(num));
    prints("\n");
    return 0;
}