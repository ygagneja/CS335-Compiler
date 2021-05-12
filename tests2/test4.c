//Program to test short-circuiting for && and ||

int main(){
    int a = 1;
    int b = 2;
    bool res = false;
    printi(res);
    if(a == 1 && b++){
        res = true;
    }
    printi(b);
    if(a == 2 && b++){
        res = true;
    }
    printi(b);
    if(a == 2 || b++){
        res = true;
    }
    printi(b);
    if(a == 1 || b++){
        res = true;
    }
    printi(b);
    prints("\n");
    return 0;
}