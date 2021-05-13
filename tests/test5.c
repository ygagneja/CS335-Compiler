// Test for nested if else

// Redundant labels are avoided using backpatching as discussed in class
// If still some redundant labels are left in 3ac, those are cleared during code gen
// Show 3ac and asm if required
int main()
{
    int var1, var2, var3, var4;
    prints("Input the value of var1: \n");
    var1 = scani();
    prints("Input the value of var2: \n");
    var2 = scani();
    prints("Input the value of var3: \n");
    var3 = scani();
    prints("Input the value of var4: \n");
    var4 = scani();
    
    if (var1 != var2)
    {
        if (var1 > var2)
        {
            if(var3 == var4){
                if(var1 == var3)
                    prints("var2 is the smallest number\n");
                else if(var1 > var3)
                    prints("var1 is the greatest number\n");
                else
                    prints("var3 and var4 are greatest numbers\n");
            }else if(var3 > var4){
                if(var1 == var3)
                    prints("var1 and var3 are greatest numbers\n");
                else if(var1 > var3)
                    prints("var1 is the greatest number\n");
                else
                    prints("var3 is the greatest numbers\n");
            }else{
                if(var1 == var4)
                    prints("var1 and var4 are greatest numbers\n");
                else if(var1 > var4)
                    prints("var1 is the greatest number\n");
                else
                    prints("var4 is the greatest numbers\n");
            }
        }
        else
        {
            if(var3 == var4){
                if(var2 == var3)
                    prints("var1 is the smallest number\n");
                else if(var2 > var3)
                    prints("var2 is the greatest number\n");
                else
                    prints("var3 and var4 are greatest numbers\n");
            }else if(var3 > var4){
                if(var2 == var3)
                    prints("var2 and var3 are greatest numbers\n");
                else if(var2 > var3)
                    prints("var2 is the greatest number\n");
                else
                    prints("var3 is the greatest numbers\n");
            }else{
                if(var2 == var4)
                    prints("var2 and var4 are greatest numbers\n");
                else if(var2 > var4)
                    prints("var2 is the greatest number\n");
                else
                    prints("var4 is the greatest numbers\n");
            }
        }
    }
    else
    {
        prints("var1 is equal to var2.. ignoring var3 and var4\n");
    }
    return 0;
}