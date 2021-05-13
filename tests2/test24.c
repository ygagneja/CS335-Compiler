// nested loops and swith-case

int main(){
    int i, j = -1, k;
    bool pass = false;
    for(i = 0; i<10; i++){
        for(j = 0; j < 10; j++){
            switch(i){
                case 1:
                    {
                        switch (j)
                        {
                            case 1:
                                pass = true;
                                break;
                            case 2: {
                                        for(k = 0; k < 10; k++){
                                            i++;
                                            if(i == 1) j = 10;
                                        }
                                    }
                            default:
                                break;
                        }
                    }
                case 2:
                    {
                        switch (j)
                        {
                            case 1:
                                pass = true;
                                break;
                            case 2:
                            default:
                                break;
                        }
                    }
                       
            }
        }
    }
}
