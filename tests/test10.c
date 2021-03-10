typedef struct node{
    int val;
    struct node* next;
}node;


static int calls = 0;

void print(struct node * head){
  calls++;
  do{
    printf("%d ", head -> val);
    head = head -> next;
  } while(head);
  printf("\n");
}

union Data {
   int i;
   float f;
   char str[20];
};

void storage_class_specifier(){
    int x;
    int a = 32;
    register char b = 'G';
    extern int x;
    calls++;
    printf("Storage class values : %d %c %d", a, b, x);
}

// Check the enumeration list statements
enum subjects {Physics = 1, Maths = 2, Chemistry = 0};
int main(){
    // Conditional and unary expression
    int k = (1 << 10) + (int)(int)sizeof(sizeof(int));
    // struct and unary
    struct node a = {4};
    struct node b = {5};
    struct node c = {6};
    
    union Data data;
    enum subjects my_fav = Physics;

    const volatile int local = 10;
    int *ptr = (int*) &local;

    short int tmp = 3;
    short int tmp1 = 4;
    int tmp2 = 5;

    if(k > (1 << 10) + (1 & 3)){
        printf("Greater\n");
    }
    else printf("Smaller\n");

    // Union datatype
    strcpy(data.str, "MS2");
    printf("data_str : %s\n", data.str);

    // Storage class types
    storage_class_specifier();

    // Enum
    printf("My fav : %d\n", my_fav);

    // volatile
    printf("Value of ptr : %d\n", *ptr);

    // static int
    printf("No of func calls : %d\n", calls);

    // Switch statement
    switch (tmp * tmp + tmp1 * tmp1 == tmp2 * tmp2) {
       case 0: printf("Not a pythagorean triple\n");
               break;
       default: printf("Pythagorean triple");
                break;
    }
    return 0;
}
