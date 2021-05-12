// Test program to test jump statements in C

int fact(int n){
    if(n == 0) return 1;
    return n*fact(n - 1);
}

int main(){
    int i, j;

    // Break stmt
    for(i = 0; i < 1000; i++){
        printf(i);
        if(i == 2) break;
    }

    //Continue stmt
    for(i = 0; i < 3; i++){
        if(i == 1) continue;
        printi(i);
    }
    
    printi(fact(7));

    // return jump
    return 0;
}