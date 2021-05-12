// Newton Raphson code on floats

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
    float absh = (h >= 0) ? h : -h;
    while (absh >= 0.1)
    {
        h = func(x)/derivFunc(x);
  
        // x(i+1) = x(i) - f(x) / f'(x) 
        x = x - h;
    }
 
    prints("Root is:");
    printf(x);
}
 
// Driver program to test above
int main()
{
    float x0 = 20; // Initial values assumed
    newtonRaphson(x0);
    return 0;
}