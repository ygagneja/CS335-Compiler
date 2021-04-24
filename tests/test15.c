//testing array (*******not ok******)

int main(){
    int a[10];
    int mat[10][3];
    int** ptr;
    int i, j;
    for(i = 0; i < 10; i++){
        a[i] = *ptr;
    }
    a[11] = 1;
    i = a[2];
    mat[i][j] = 100;
    ptr = mat[i];
    ptr = ptr + 1;
    j = j*j + j - j/j + j;
    ptr = *ptr + **ptr + **ptr;
}