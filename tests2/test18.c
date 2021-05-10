// Large expression
void printf(char* str, ...);
void scanf(char* str, ...);

int main(){
    
    int a = 5, b = 3, c = 1, d = 11, e = 12, f = 20, g = 32, h = 10;
    int res = a + b * c - d + e * f % g - h;
    printf("Result is  %d",res);
    return 0;
}