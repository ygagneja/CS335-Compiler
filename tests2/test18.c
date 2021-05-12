// Large expression, with more permutations handled

int main(){
    
    int a = 5, b = 3, c = 1, d = 11, e = 12, f = 20, g = 32, h = 10;
    float res1 = -a * b + c / d; // has a unary -
    float res2 = a / b * c - d; // div should happen first, and flooring it to int
    int res3 = a * b - c * d; // First multiplications, then subtraction
    float res4 = f / b / c / e; // Left to right?
    float res5 = a % b / c * d;
    int res6 = h++ + g++; 
    int res7;
    h--;
    g--;
    res7 = ++h + ++g; // Note difference between res6 and res7
    printf(res1);
    printf(res2);
    printi(res3);
    printf(res4);
    printf(res5);
    printi(res6);
    printi(res7);
    return 0;
}