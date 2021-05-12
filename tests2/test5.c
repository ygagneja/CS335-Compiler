// Test for nested if else

int main()
{
    int var1, var2, var3, var4;
    prints("Input the value of var1: ");
    var1 = scani();
    prints("Input the value of var2: ");
    var2 = scani();
    prints("Input the value of var3: ");
    var3 = scani();
    prints("Input the value of var4: ");
    var4 = scani();
    
    if (var1 != var2)
    {
        if (var1 > var2)
        {
            if(var3 == var4){
                if(var1 == var3)
                    prints("var2 is the smallest number");
                else if(var1 > var3)
                    prints("var1 is the greatest number");
                else
                    prints("var3 and var4 are greatest numbers");
            }else if(var3 > var4){
                if(var1 == var3)
                    prints("var1 and var3 are greatest numbers");
                else if(var1 > var3)
                    prints("var1 is the greatest number");
                else
                    prints("var3 is the greatest numbers");
            }else{
                if(var1 == var4)
                    prints("var1 and var4 are greatest numbers");
                else if(var1 > var4)
                    prints("var1 is the greatest number");
                else
                    prints("var4 is the greatest numbers");
            }
        }
        else
        {
            if(var3 == var4){
                if(var2 == var3)
                    prints("var1 is the smallest number");
                else if(var2 > var3)
                    prints("var2 is the greatest number");
                else
                    prints("var3 and var4 are greatest numbers");
            }else if(var3 > var4){
                if(var2 == var3)
                    prints("var2 and var3 are greatest numbers");
                else if(var2 > var3)
                    prints("var2 is the greatest number");
                else
                    prints("var3 is the greatest numbers");
            }else{
                if(var2 == var4)
                    prints("var2 and var4 are greatest numbers");
                else if(var2 > var4)
                    prints("var2 is the greatest number");
                else
                    prints("var4 is the greatest numbers");
            }
        }
    }
    else
    {
        prints("var1 is equal to var2.. ignoring var3 and var4.");
    }
    return 0;
}