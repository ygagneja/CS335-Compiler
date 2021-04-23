//testing array (*******not ok******)

int main(){
    int a[10];
    int i, j;
    for(i = 0; i < 10; i++){
        a[i] = i*i;
    }
    a[11] = 1; // should give an error
}