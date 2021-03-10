#include "stdio.h"
#include "stdlib.h"
#include "string.h"

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

int x;
void storage_class_specifier(){
    calls++;
    int a = 32;
    register char b = 'G';
    extern int x;
    x = 40;
    printf("Storage class values : %d %c %d", a, b, x);
}

// Check the enumeration list statements
enum subjects {Physics = 1, Maths = 2, Chemistry = 0};
int main(){
    // Conditional and unary expression
    int k = (1 << 10) + (int)(int)sizeof(sizeof(int));
    if(k > (1 << 10) + (1 & 3)){
        printf("Greater\n");
    }
    else printf("Smaller\n");

    // struct and unary
    struct node a;
    a.val = 4;
    struct node b;
    b.val = 5;
    struct node c;
    c.val = 6;
    a.next = &b, b.next = &c, c.next = NULL;
    print(&a);

    // Union datatype
    union Data data;
    strcpy(data.str, "MS2");
    printf("data_str : %s\n", data.str);

    // Storage class types
    storage_class_specifier();

    // Enum
    enum subjects my_fav = Physics;
    printf("My fav : %d\n", my_fav);

    // volatile
    const volatile int local = 10;
    int *ptr = (int*) &local;
    printf("Value of ptr : %d\n", *ptr);

    // static int
    printf("No of func calls : %d\n", calls);

    // Switch statement
    short int tmp = 3;
    short int tmp1 = 4;
    int tmp2 = 5;
    switch (tmp * tmp + tmp1 * tmp1 == tmp2 * tmp2) {
       case 0: printf("Not a pythagorean triple\n");
               break;
       default: printf("Pythagorean triple");
                break;
    }
    return 0;
}
