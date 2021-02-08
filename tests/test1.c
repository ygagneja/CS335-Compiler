int main(){
    int i = 0;
    for (i=0; i<10; i++){
        if (i == 2)
            printf ("hey there");
    }
    float a = 2;
    while (a--){
        break;
    }
    int a = 5+10;
        int c = a;
    float x = c&a; // one line comment
      x += a - c<<5; 
    /* multi 
        line comment */
    $$ = 6; // should throw error at line 17

    return 0;
}