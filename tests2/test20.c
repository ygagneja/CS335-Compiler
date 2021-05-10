// Newton Raphson code on floats

void printf(char* str, ...);
void scanf(char* str, ...);

//The function is x^3 - x^2  + 2
float func(float x)
{
    return x*x*x - x*x + 2;
}
 
// Derivative of the above function which is 3*x^x - 2*x
float derivFunc(float x)
{
    return 3*x*x - 2*x;
}
 
// Function to find the root
void newtonRaphson(float x)
{
    float h = func(x) / derivFunc(x);
    while (abs(h) >= 0.001)
    {
        h = func(x)/derivFunc(x);
  
        // x(i+1) = x(i) - f(x) / f'(x) 
        x = x - h;
    }
 
    printf("Root is %d",x);
}
 
// Driver program to test above
int main()
{
    float x0 = -20; // Initial values assumed
    newtonRaphson(x0);
    return 0;
}