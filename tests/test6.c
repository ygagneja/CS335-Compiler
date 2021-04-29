void printf(char* str, ...);
void scanf(char* str, ...);

int MAX_SIZE = 500;

int a[2][2];
int b[2][2];

int mul_and_xor(int r1, int c1, int r2, int c2){
    int res = 0;
    int i, j, k, counter;
    for(i = 0; i < r1; i++){
      for(j = 0; j < c2; j++){
        counter = 0;
        for(k = 0; k < c1; k++) counter += a[i][k] * b[k][j];
        res = res ^ counter;
      }
    }
    return res;
}

void Check(){
    int c = 5;
    int *ptr = &c;
    int k = (1 << 10) - 1;
    int cnt = 0;
    int i = 0;
    if(((*ptr) ^ (*ptr)) != 0) printf("Wrong_result\n");
    else printf("Correct result\n");
    for(i = 0; i < 10; i++) if((1 << i) & k) cnt++;
    if(cnt > 10){
        printf("More no of ones\n");
    }
    else if(cnt < 10){
        printf("Less no of ones\n");
    }
    else if(cnt == 10) printf("Correct no of ones\n");

}

int MAX_GIRLFRIENDS = 5;
struct girlfriend_list {
    char *name;
};
struct person {
    int age;
    char* name;
    struct girlfriend_list* gf;
};


int main(){
    struct person me;
    int null = 0;
    int r1 = 2, c1 = 2, r2 = 2, c2 = 2;
    int res = mul_and_xor(r1, c1, r2, c2);
    me.age = 21; 
    me.name = "Yuvraj";
    me.gf = null;
    Check();
    return 0;
}
