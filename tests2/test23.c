//testing if-else and loops

int main(){

    //Testing IF ELSE type statements
    int i = scani();
    int t = 50;
    int j;
    if(!i){
        if(i == 5)
            prints("i is equal to 5");
        else
            prints("i not equal to 5");
    }
        
    if(t > 10)
        prints("\nt is big");

    i = 1;
    switch (i)
    {
    case 1: {
                prints("\ni is 1");
                prints("\nMultiple expressions");
                break;
            }
    case 2: {
                prints("\ni is 1");
                break;
            }
    default:
        break;
    }

    i = (i == 1) ? 5 : -10;



    // Testing different types of loops
    i = 5;
    while(i > 0){
        prints("\nvalue of i = ");
        printi(i);
        i--;
    }

    for(i = 10; i > 5; i--){
        prints("\nvalue of i = ");
        printi(i);
    }

    for(i = 10; i > 5;){
        prints("\nvalue of i = ");
        printi(i);
        i--;
    }

    i = 8;
    for( ; i > 5;){
        prints("\nvalue of i = ");
        printi(i);
        i--;
    }

    i = 10;
    do{
        prints("\nvalue of i = ");
        printi(i);
        i++;
    }while(i < 15);

    for(i = 0; i < 2; i++){
        for(j = 1; j < 3; j++){
            prints("\nvalue of i = ");
            printi(i);
            prints("\nvalue of j = ");
            printi(j);
        }
    }
}