// Checking precedence of unary and binary minus
int main(){
    int a = 3,b = 7, c = 10, d = 12, e = 20;
    int res = -(-a-b)-(c - d);
    printi(res); // Expected output : 12
    prints("\n");
    return 0;
}