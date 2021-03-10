int main(){

    //Testing IF ELSE type statements
    int i = 5;
    if(!i)
        if(i == 5)
            printf("i is equal to 5\n");
        else
            printf("i not equal to 5\n");
        
    long t = 50;
    if(t > 10)
        printf("t is big\n");

    i = 1;
    switch (i)
    {
    case 1:
        printf("i is 1\n");
        printf("Multiple expressions\n");
        break;
    case 2:
        printf("i is 1\n");
        break;
    default:
        break;
    }



    // Testing different types of loops
    i = 5;
    while(i > 0){
        printf("value of i = %d\n", i);
        i--;
    }

    for(i = 10; i > 5; i--){
        printf("value of i = %d\n", i);
    }

    for(i = 10; i > 5;){
        printf("value of i = %d\n", i);
        i--;
    }

    i = 8;
    for( ; i > 5;){
        printf("value of i = %d\n", i);
        i--;
    }

    i = 10;
    do{
        printf("value of i = %d\n", i);
        i++;
    }while(i < 15);

    for(i = 0; i < 2; i++){
        for(int j = 1; j < 3; j++){
            printf("i = %d and j = %d\n", i, j);
        }
    }
}