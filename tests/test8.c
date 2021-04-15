void printf(char* str, ...);
void scanf(char* str, ...);

// Test program to test jump statements in C

//function to calculate factorial of n
int fact(int n){
    if(n == 0) return 1;
    return n*fact(n - 1);
}

int main(){
    int i, j;

    // Break stmt
    for(i = 0; i < 100; i++){
        printf("%d\n", i);
        if(i == 2) break;
    }

    //Continue stmt
    for(i = 0; i < 3; i++){
        if(i == 1) continue;
        printf("%d\n", i);
    }

    //goto jump stmt
    for(i = 0; i < 5; i++){
        if(i == 3)
            goto Print_three;
        printf("%d\n", i);
    }
    Print_three:
        printf("3\n");

    // return jump
    printf("%d\n", fact(7));
    return 0;
}