// Test program to test jump statements in C

int main(){
    int i, j;

    // Break stmt
    for(i = 0; i < 1000; i++){
        printi(i);
        if(i == 2) break;
    }

    //Continue stmt
    for(i = 0; i < 3; i++){
        if(i == 1) continue;
        printi(i);
    }

    prints("\n");
    // return jump
    return 0;
}