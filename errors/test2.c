// this should work as the control flow is top to down
int main(){
    float a = 1.1;
    int b = 2;
    bool res = false;
    printi(res);
    if(a == 1 && b == 3){
        res = true;
    }
    printi(res);
    if(a == 2 || b == 2){
        res = true;
    }
    printi(res);// should print 1 but output is 0
    return 0;
}
//Assembly wrong.. 
//after Line 106 no further instruction to jump to. (for all float type instr of this type add jump instr at this line)
// in Label_18 and Label_31, not further instruction to jump to.