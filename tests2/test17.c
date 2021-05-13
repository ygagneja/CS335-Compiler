// Program to test Function with >7 parameters (ADV: Optimized Reg Allocation)

/* function returning the max between 8 positive numbers */
int max(int num1, int num2, int num3, int num4, float num5, float num6, float num7, float num8) {

   /* local variable declaration */
    int result = 0;
    if (num1 > result)
        result = num1;
    if(num2 > result)
        result = num2;
    if(num3 > result)
        result = num3;
    if(num4 > result)
        result = num4;
    if(num5 > result)
        result = num5;
    if(num6 > result)
        result = num6;
    if(num7 > result)
        result = num7;
    if(num8 > result)
        result = num8;
 
   return result; 
}

int main () {

   int ret;
 
   ret = max(100, 50, 20, 150, 180.2, 200.5, 112.5, 121.434);
 
   prints("Max value is: ");
   printi(ret);
 
   return 0;
}