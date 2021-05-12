//testing if-else and loops

int main(){

    //Testing IF ELSE type statements
    int i = 5;
    int t = 50;
    int j;
    if(!i)
        if(i == 5)
            prints("i is equal to 5");
        else
            prints("i not equal to 5");
        
    if(t > 10)
        prints("t is big");

    i = 1;
    switch (i)
    {
    case 1:{
            prints("i is 1");
            prints("Multiple expressions");
            break;
        }
    case 2:{
            prints("i is 1");
            break;
        }
    default:
        break;
    }

    i = (i == 1) ? 5 : -10;



    // Testing different types of loops
    i = 5;
    while(i > 0){
        prints("value of i = ");
        printi(i);
        i--;
    }

    for(i = 10; i > 5; i--){
        prints("value of i = ");
        printi(i);
    }

    for(i = 10; i > 5;){
        prints("value of i = ");
        printi(i);
        i--;
    }

    i = 8;
    for( ; i > 5;){
        prints("value of i = ");
        printi(i);
        i--;
    }

    i = 10;
    do{
        prints("value of i = ");
        printi(i);
        i++;
    }while(i < 15);

    for(i = 0; i < 2; i++){
        for(j = 1; j < 3; j++){
            prints("value of i = ");
            printi(i);
            prints("value of j = ");
            printi(j);
        }
    }
}