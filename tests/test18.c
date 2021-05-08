int main(){
    float x = 1.2;
    int y = 1, z = 1, i = 1, j = 1;
    bool a = 1;
    a = x ? 1 : 2;
    y = x==1 ? z+++i+j : z*(i++)/(++j);
    for(i = 0, j = 0, y = 0; i < 10 && j < 10; j += (i++)*2){
        switch (i)
        {
        case 1:
            z = ++y*i+1;
            break;
        case 2:
            z = y >= 10 ? i++ : ++j;
            break;
        case 3:
            if(y) x++;
            break;
        default:
            break;
        }
    }
}
