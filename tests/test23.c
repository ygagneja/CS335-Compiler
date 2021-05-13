//testing switch case and different loop types

int main(){

    //Testing IF ELSE type statements
    int i, j;
    prints("Enter two numbers :\n");
    i = scani();
    j = scani();

    switch (i)
    {
    case true: {
                prints("i is 1\n");
                switch (j)
                {
                    case 4.0 : prints("j is 4\n");
                    case 2 : prints("j is 2\n");
                    case 'a' : prints("j is ascii val of a\n");
                    case i :prints("j is equal to i\n");
                }
                break;
            }
    case 2: {
                prints("i is 2\n"); // fall through
            }
    case i: {
                prints("i is not 1\n");
                break;
            }
    default:
        break;
    }

    // Testing different types of loops
    i = 5;
    while(i > 0){
        printi(i);
        i--;
    }
    prints("\n");

    for(i = 10; i > 5; i--){
        printi(i);
    }
    prints("\n");

    for(i = 10; i > 5;){
        printi(i);
        i--;
    }
    prints("\n");

    i = 8;
    for( ; i > 5;){
        printi(i);
        i--;
    }
    prints("\n");

    i = 10;
    do{
        printi(i);
        i++;
    }while(i < 15);
    prints("\n");
}