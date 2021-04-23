// loops

int main(){
    int a = 1;
    int i, j, k;
    int res = 0;
    for(k = 0; k<10; k++){
        if(k % 2) a++;
        else a++;
    }
    for(i = 1; i < 10; i++){
        for(j = 0; j < i; j++){
            res++;
            ++res;
            res--;
            --res;
        }
    }
    if(i%2) a++;
    else a--;
    if(a == 2) a++;
    res = 10;
    while(res--){
        for(i = 1; i < 10; i++){
            for(j = 0; j < i; j++){
                res--;
            }
        }
    }

    res = 9;
    do{
        res--;
    }while(res);
}