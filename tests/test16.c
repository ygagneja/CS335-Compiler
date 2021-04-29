//Switch cases

int main(){
    int a, b, c, k, res = 30, j;
    int i = 10;
    a = 10, b = 10, c = 2;
    switch (i+a+b*c)
    {
    case 10:
        for(k = 0; k<10; k++){
            if(k % 2) a++;
            else a++;
        }
        break;
    case 40:
        if(k % 2) a++;
        else a--;
        break;
    case 50:
        while(res--){
            for(i = 1; i < 10; i++){
                for(j = 0; j < i; j++){
                    res--;
                }
            }
        }
        break;
    default:
        break;
    }
}