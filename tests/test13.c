// Recursion

int fact(int a){
    if(a == 0) return 1;
    return a*fact(a-1);
}

int main(){
    int b = fact(10);
    return b;
}