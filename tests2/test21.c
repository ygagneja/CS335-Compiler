// Miscellaneous (func+loops+switch-case+conditional exp + arrays)

int x(){
    return 1;
}
int y(){
    return 1;
}
int main(int i){
    char* ptr = 1;
    bool a = 1, b = false;
    float x = 1.2;
    char arr[10];
    int z = 5;
    a = x ? 1 : 2;
    while (b == 2){
        if (x >= 10){
            x = y();
        }
        if (x < 10){
            break;
        }
    }
    i = b + x*i;
    switch(1){
        case 1 : {break;}
        case 2 : {break;}
        case 3 :{
                    a = x ? 1 : 2;
                    break;
                }
        default : {break;}
    }
    while (true){
        if (true) break;
        if (false) continue;
    }
    for (i=0; i<10; i++){
        arr[i] = i;
    }
    if (true){
        arr[0] = 1;
    }
    else {
        arr[1] = 2;
    }
    return 0;
}