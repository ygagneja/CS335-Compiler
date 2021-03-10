const int MAX_SIZE = 500;

int a[2][2] = {{1, 2}, {3, 5}};
int b[2][2] = {{1, 4}, {5, 8}};
// r2 = c1
int mul_and_xor(int r1, int c1, int r2, int c2){
    int res = 0;
    for(int i = 0; i < r1; i++){
      for(int j = 0; j < c2; j++){
        int counter = 0;
        for(int k = 0; k < c1; k++) counter += a[i][k] * b[k][j];
        res = res ^ counter;
      }
    }
    return res;
}

// Checks Unary expression, relational_expression, shift_expression, equality expression etc.
void Check(){
    int c = 5;
    int *ptr = &c;
    if(((*ptr) ^ (*ptr)) != 0) printf("Wrong_result\n");
    else printf("Correct result\n");
    int k = (1 << 10) - 1;
    int cnt = 0;
    for(int i = 0; i < 10; i++) if((1 << i) & k) cnt++;
    if(cnt > 10){
        printf("More no of ones\n");
    }
    else if(cnt < 10){
        printf("Less no of ones\n");
    }
    else if(cnt == 10) printf("Correct no of ones\n");

}

const int MAX_GIRLFRIENDS = 5;
struct person {
    int age;
    char* name;
    struct girlfriend_list* gf;
};

struct girlfriend_list {
    char *name;
};


int main(){
    int ans = (int) sizeof(float) + (int) sizeof(double);
    printf("%d\n", ans);

  
    struct person me;
    me.age = 10, me.name = "Yuvraj";

    struct girlfriend_list mine;
    mine.name = "No_one";

    me.gf = &mine;

    struct person * ptr = &me;
    printf("Person : %s First_gf : %s\n", ptr -> name, ptr -> gf -> name);

    Check();
    int r1 = 2, c1 = 2, r2 = 2, c2 = 2;
    int res = mul_and_xor(r1, c1, r2, c2);
    printf("%d\n", res);
    return 0;
}
