// Checking precedence of unary and binary minus
int main(){
    int a = 3,b = 7, c = 10, d = 12;
    int res = -(-a-b)-(c-d);
    printi(res); // Expected output : 12
    res = +(a+b)+(-c-d); // Expected output : -12
    printi(res);
    prints("\n");
    return 0;
}