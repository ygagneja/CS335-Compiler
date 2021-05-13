// Large expression, with more permutations handled

// observe precedence and optimized register allocation using next use

int main(){
    
    int a = 5, b = 3, c = 1, d = 11, e = 12, f = 20, g = 32, h = 10;
    a = a * b - c * d;
    b = f / b / c / e;
    c = a*b + c<<2 - d/h + (a==b);
    d = h++ + g++; 
    h--; g--;
    e = ++h + ++g;
    printi(a); // should be 4
    printi(b); // should be 0
    printi(c); // should be 2
    printi(d); // should be 42
    printi(e); // should be 44
    printi(g); // should be 33
    printi(h);  // should be 11
    prints("\n");
    return 0;
}