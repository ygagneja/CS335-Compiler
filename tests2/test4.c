//Program to test short-circuiting for && and ||

int main(){
    float a = 1.1;
    int b = 2;
    bool res = false;
    if(a == 1 && b == 3){
        res = true;
    }
    if(a == 2 || b == 2){
        res = true;
    }
    return 0;
}