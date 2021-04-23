//test short-circuiting 

int main(){
    int a = 1, b = 2;
    bool res = false;
    if(a == 1 && b == 3){
        res = true;
    }
    if(a == 1 && b == 2){
        res = true;
    }
    if(a == 2 && b == 3){
        res = true;
    }
    if(a == 2 && b == 2){
        res = true;
    }
    if(a == 1 || b == 3){
        res = true;
    }
    if(a == 1 || b == 2){
        res = true;
    }
    if(a == 2 || b == 3){
        res = true;
    }
    if(a == 2 || b == 2){
        res = true;
    }
}