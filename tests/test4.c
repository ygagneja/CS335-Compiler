//Program to test short-circuiting for && and ||

int main(){
    int a = 1;
    int b = 2;
    bool res = false;
    printi(res); // should be 0
    if(a == 1 && b++){
        res = true;
    }
    printi(b); // should be 3
    if(a == 2 && b++){
        res = true;
    }
    printi(b); // should be 3
    if(a == 2 || b++){
        res = true;
    }
    printi(b); // should be 4
    if(a == 1 || b++){
        res = true;
    }
    printi(b); // should be 4
    prints("\n");
    return 0;
}